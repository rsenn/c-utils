/* fixup-wrapped-strings.js
 *
 * clang-format (or similar) sometimes wraps a single logical C string
 * across several adjacent string-literal tokens purely to fit the
 * column limit, e.g.:
 *
 *   "  -h, --help              Show "
 *   "this help\n",
 *
 * Since adjacent string literals are implicitly concatenated by C,
 * that's one string. This script merges every such run of adjacent
 * literals (separated only by whitespace, no comma/other token
 * between them) back into a single literal, splitting the result
 * again only at each "\n" escape it actually contains -- never at an
 * arbitrary column -- so:
 *
 *   "  -h, --help              Show this help\n"
 *
 * A run that already contains more than one "\n" is re-emitted as one
 * literal per line, each starting on its own line at the same
 * indentation the run started at.
 *
 * Usage (from the qjsm interpreter, see ../plot-cv/quickjs):
 *
 *   qjsm scripts/fixup-wrapped-strings.js *.c
 *
 * Rewrites each given file in place; prints the files it changed.
 *
 * Detect-only mode: instead of fixing files, recursively find every
 * *.c/*.h file under the project folder (or under the given paths)
 * that contains at least one such run and list it, without touching
 * anything:
 *
 *   qjsm scripts/fixup-wrapped-strings.js --list [dir-or-file ...]
 */
import { readFileSync, writeFileSync, readdirSync, statSync } from 'fs';
import { join, extname } from 'path';
import CLexer from 'lexer/c.js';

const SKIP_DIRS = new Set(['.git', 'build', '3rdparty', 'node_modules', 'CMakeFiles']);

function stringLiteralContent(lexeme) {
  const start = lexeme[0] === '"' ? 1 : 2; /* optional prefix char before the opening quote */
  return lexeme.slice(start, lexeme.length - 1);
}

/* Split raw (still-escaped) string content after every "\n" escape,
 * without splitting inside any other escape sequence. */
function splitOnNewlineEscapes(content) {
  const parts = [];
  let cur = '';

  for(let i = 0; i < content.length; ++i) {
    const c = content[i];

    if(c === '\\' && i + 1 < content.length) {
      const next = content[++i];
      cur += c + next;

      if(next === 'n') {
        parts.push(cur);
        cur = '';
      }

      continue;
    }

    cur += c;
  }

  if(cur.length)
    parts.push(cur);

  return parts;
}

function indentBefore(src, pos) {
  const lineStart = src.lastIndexOf('\n', pos - 1) + 1;
  return src.slice(lineStart, pos);
}

/* Indentation to use for a run's continuation lines. Prefer the
 * indentation already used before the run's *second* literal, when
 * that literal starts its own line (the common wrapped-argument-list
 * style, e.g. buffer_putm_internal's one-literal-per-line calls) --
 * that preserves the exact original alignment. Otherwise (e.g. a
 * macro call whose first literal follows the macro name on the same
 * line, "TESTLOG_STR(\"foo\\n\"") fall back to just the leading
 * whitespace of the run's own line: the text before a literal is not
 * necessarily whitespace-only, and repeating it verbatim would
 * duplicate real code onto every continuation line. */
function indentForRun(tokens, i, last, src) {
  for(let k = i + 1; k <= last; ++k) {
    if(tokens[k].type !== 'string_literal')
      continue;

    const prefix = indentBefore(src, tokens[k].charRange[0]);

    if(/^[ \t]*$/.test(prefix))
      return prefix;

    break;
  }

  return /^[ \t]*/.exec(indentBefore(src, tokens[i].charRange[0]))[0];
}

/* Yield [start, end] token-index pairs, one per run of 2+ adjacent
 * string_literal tokens (separated only by whitespace). A lone
 * literal (no concatenation) is not a run and is not yielded. */
function* findRuns(tokens) {
  for(let i = 0; i < tokens.length; ++i) {
    if(tokens[i].type !== 'string_literal')
      continue;

    let last = i;
    let j = i + 1;

    for(;;) {
      let k = j;

      while(k < tokens.length && tokens[k].type === 'whitespace')
        ++k;

      if(k < tokens.length && tokens[k].type === 'string_literal') {
        last = k;
        j = k + 1;
        continue;
      }

      break;
    }

    if(last !== i)
      yield [i, last];

    i = last;
  }
}

function tokenize(src, fileName) {
  const lexer = new CLexer(src, CLexer.LONGEST, fileName);
  return [...lexer];
}

/* The replacement text for a run: its literals' contents concatenated
 * and re-split at each "\n" escape. For a run that's already in that
 * exact shape (e.g. a deliberate multi-line message where every
 * non-last literal already ends in "\n"), this comes out byte-for-byte
 * identical to the original span -- callers compare against that span
 * to tell a run that needs fixing from one that's already fine. */
function runReplacement(tokens, i, last, src) {
  let content = '';

  for(let k = i; k <= last; ++k)
    if(tokens[k].type === 'string_literal')
      content += stringLiteralContent(tokens[k].lexeme);

  const parts = splitOnNewlineEscapes(content);
  const indent = indentForRun(tokens, i, last, src);

  return parts.map((p, idx) => (idx === 0 ? '' : '\n' + indent) + '"' + p + '"').join('');
}

/* Like findRuns(), but only the runs whose replacement actually
 * differs from the source -- i.e. runs that need fixing. */
function* findFixableRuns(tokens, src) {
  for(const [i, last] of findRuns(tokens)) {
    const [runStart] = tokens[i].charRange;
    const [, runEnd] = tokens[last].charRange;

    if(runReplacement(tokens, i, last, src) !== src.slice(runStart, runEnd))
      yield [i, last];
  }
}

function countRuns(src, fileName) {
  return [...findFixableRuns(tokenize(src, fileName), src)].length;
}

function fixupSource(src, fileName) {
  const tokens = tokenize(src, fileName);

  let out = '';
  let pos = 0;

  for(const [i, last] of findFixableRuns(tokens, src)) {
    const [runStart] = tokens[i].charRange;
    const [, runEnd] = tokens[last].charRange;

    out += src.slice(pos, runStart);
    out += runReplacement(tokens, i, last, src);

    pos = runEnd;
  }

  out += src.slice(pos);

  return out;
}

function fixupFile(path) {
  const src = readFileSync(path, 'utf-8');
  let out;

  try {
    out = fixupSource(src, path);
  } catch(e) {
    console.log(`skipping '${path}': ${e.message}`);
    return false;
  }

  if(out !== src) {
    writeFileSync(path, out);
    return true;
  }

  return false;
}

/* Recursively yield every *.c/*.h file under `dir`, skipping VCS/build/
 * vendor directories and anything starting with '.'. */
function* walk(dir) {
  let entries;

  try {
    entries = readdirSync(dir, { withFileTypes: true });
  } catch(e) {
    return;
  }

  for(const entry of entries) {
    if(entry.name.startsWith('.'))
      continue;

    const path = join(dir, entry.name);

    if(entry.isDirectory()) {
      if(SKIP_DIRS.has(entry.name))
        continue;

      yield* walk(path);
    } else if(entry.isFile() && (extname(entry.name) === '.c' || extname(entry.name) === '.h')) {
      yield path;
    }
  }
}

function listFiles(paths) {
  for(const path of paths) {
    let src;

    try {
      src = readFileSync(path, 'utf-8');
    } catch(e) {
      console.log(`skipping '${path}': ${e.message}`);
      continue;
    }

    let n;

    try {
      n = countRuns(src, path);
    } catch(e) {
      console.log(`skipping '${path}': ${e.message}`);
      continue;
    }

    if(n > 0)
      console.log(`${path}: ${n}`);
  }
}

function isDir(path) {
  try {
    return statSync(path).isDirectory();
  } catch(e) {
    return false;
  }
}

const args = scriptArgs.slice(1);
const listMode = args[0] === '--list' || args[0] === '-l';
const rest = listMode ? args.slice(1) : args;

if(listMode)
  listFiles(rest.length ? rest.flatMap((p) => (isDir(p) ? [...walk(p)] : [p])) : [...walk('.')]);
else
  for(const file of rest)
    if(fixupFile(file))
      console.log('fixed', file);

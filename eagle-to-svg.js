const DOMParser = require("xmldom").DOMParser;
const fs = require("fs");
const util = require("util");
const Util = require("./utils/util.js");
const dom = require("./utils/dom.es5.js");
const eagle = require("./utils/eagle.js");
const style = require("ansi-styles");
const { Element, Line, Point, PointList, Rect } = dom;
const colors = eagle.eagleProps.colors;
const SVG = require("@svgdotjs/svg.js");

var data = fs.readFileSync("/home/roman/Dokumente/Sources/pictest/eagle/PIC18F2550-USB+ICSP-Board.brd").toString();
var xml = new DOMParser().parseFromString(data);
var doc = xml.documentElement;

global.document = doc;
console.log(eagle);
eagle.eagleProps.tCoords = coord => Math.floor((coord * 2) / 2.54) * 0.5;

function xml2obj(node, parent, pobj) {
  let a = Object.fromEntries(
    Object.entries(dom.Node.attrs(node)).map(([key, value]) => [
      key,
      isNaN(parseFloat(value)) || key == "name" ? value : parseFloat(value)
    ])
  );
  var obj;
  if(node.tagName !== undefined) {
    a.tag = node.tagName;
    obj = eagle.default(a, pobj);
    if(obj !== null) a = obj;
  } else return null;
  var children = [];
  if(node.childNodes !== undefined && node.childNodes !== null) {
    for(let child of Array.from(node.childNodes)) {
      const obj = xml2obj(child, node, a);
      if(obj !== null) children.push(obj);
    }
  }
  if(children.length) a.children = children;
  return a;
}

var out = xml2obj(doc);
var elements = eagle.Elements;
/*
for (let id in elements) {
  const e = elements[id];
  console.log("e;", e);
  console.log("package:", e.getPackage());
  console.log("library:", e.getLibrary());
}
*/
// var positionedElements = eagle.allElements.filter(e => e.x !== undefined || e.x1 !== undefined);

// console.log("obj: ", util.inspect(eagle.ContactRefs, { colors: true, depth: 10 }));
//console.log("obj: ", util.inspect(eagle.Wires, { colors: true, depth: 10 }));

// for(let key in eagle.instances) {
//   const list = eagle.instances[key];
//   console.log("key: ", key);
//   console.log("list: ", list.length ||  Object.keys(list));
// }
console.log("Finished!");

const layers = Object.values(eagle.instances.Layer).filter(l => l.objects.length > 0);

layers.forEach(layer => {
  console.log("Layer: ", layer.name);

  if(layer.color !== undefined) {
    const color = layer.getColor();
    console.log(
      "Color: ",
      style.color.ansi.hex(colors[layer.color & ~0x08]) /* +
      style.bgColor.ansi.hex(colors[layer.color|0x08])*/ +
        " XXXX" +
        style.color.close +
        style.bgColor.close +
        "\t\t(" +
        (layer.getColor() || layer.color) +
        ")"
    );
  }
  // console.log("Fill: ", layer.fill, layer.getFill());
});
console.log("Top", util.inspect(eagle.Layers.Top, { colors: true, depth: 2 }));
console.log("Bottom", util.inspect(eagle.Layers.Bottom, { colors: true, depth: 2 }));
console.log("Signals", util.inspect(eagle.Signals, { colors: true, depth: 2 }));

// new document
var draw = SVG();

var lines = {};
var ptWire = {};

for (let signal of Object.values(eagle.instances.Signal)) {
  lines[signal.name] = signal.wires.map(w => {
    const line = new Line(w);
    line.toPoints().forEach(pt => {
      const key = pt.toString();
      if(!ptWire[key]) ptWire[key] = [];
      ptWire[key].push(w);
    });
    line.wire = w;
    w.line = line;
    return line;
  });
}

function findLine(lineArr, pts, exclude, onFound = (line, pt, pti) => {}) {
  if(pts.toPoints !== undefined) pts = pts.toPoints();
  else if(pts.length === undefined) pts = [pts];

  for(let i = 0; i < lineArr.length; i++) {
    const line = lineArr[i];
    if(i == exclude) continue;
    if(line === null) continue;
    for(let j = 0; j < pts.length; j++) {
      const pt = pts[j];
      if(line.x1 == pt.x && line.y1 == pt.y) {
        onFound(line, pt, 0, i, j);
        return i;
      }
      if(line.x2 == pt.x && line.y2 == pt.y) {
        onFound(line, pt, 1, i, j);
        return i;
      }
    }
  }
  return -1;
}

for (let signal in lines) {
  let l = [...lines[signal]].sort((a, b) => a.bounds().x1 - b.bounds().x1);
  let pl = l.map(l => l.toPoints());
  let w = l.map(l => l.wire);
  console.log("Signal: ", signal, util.inspect({ l }, { colors: true, depth: 2 }));

  let polylines = [];

  //  let p = l.reduce((acc,l) => ([...acc,...l.toPoints()]), []);

  for(let idx = 0; idx < l.length; idx++) {
    let found = idx;
    if(l[idx] === null) continue;
    polylines.unshift(new PointList());
    do {
      idx = found;
      const line = l[idx];
      found = -1;
      if(line) {
        let pts = line.toPoints();

        for(let point of pts) {
          if(polylines[0].length == 0 || !point.equal(polylines[0].last())) polylines[0].push(point);
        }

        l[idx] = null;
        found = findLine(l, pts, idx, (line, pt, ab, linei, ptidx) => {
          if(ab > 0) l[linei].swap();
          console.log("found: ", pts.toString(), line.toString());
        });
      }
      console.log("found:", idx, found);
    } while(found != -1);
  }

  for(let pline of polylines) {
    var polyline = draw
      .polyline(pline.toString())
      .fill("none")
      .stroke({ width: 1 });

    console.log("points " + signal + ": ", polyline);
  }
}
/*
let wires = eagle.Wires.map(w => { let wire = new dom.Line(w); return Object.assign(wire, w);   });
console.log(wires);
*/
// //console.log("eagle: ", util.inspect(eagle.instances, { colors: true, depth: 2 }));
// console.log("Top", util.inspect(layers, { colors: true, depth: 2 }));
// console.log(
//   "Top",
//   util.inspect(
//     layers.map(l => l.name),
//     { colors: true, depth: 2 }
//   )
// );

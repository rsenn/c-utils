
const DOMParser = require("xmldom").DOMParser;
const fs = require("fs");
const util = require("util");
const Util = require("./utils/util.js");
const dom = require("./utils/dom.es5.js");
const eagle = require("./utils/eagle.js");
const jsdom = require("jsdom");
const { JSDOM } = jsdom;

const style = require("ansi-styles");
const { Element, Line, Point, PointList, Rect } = dom;
const colors = eagle.eagleProps.colors;
const hu = require("./utils/hu.js");

var data = fs.readFileSync("/home/roman/Dokumente/Sources/pictest/eagle/PIC18F2550-USB+ICSP-Board.brd").toString();
var xml = new DOMParser().parseFromString(data);
var doc = xml.documentElement;

var DOM = new JSDOM("<svg></svg>");
var window = DOM.window;
var document = window.document;

hu.setWindow(window);
hu.setDocument(document);

eagle.eagleProps.tCoords = coord => Math.floor((coord * 2) / 2.54) * 0.5;
var group = hu("<g>");

/**
 * xml2obj
 *
 * @param      {<type>}  node    The node
 * @param      {<type>}  parent  The parent
 * @param      {<type>}  pobj    The pobj
 * @return     {<type>}  { description_of_the_return_value }
 */
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

var lines = {};
var ptWire = {};

let layers = Object.values(eagle.instances.Layer).sort((a, b) => b.number - a.number);

layers = ["Bottom", "Top", "Measures", "tNames", "bNames", "tValues", "bValues"].map(name => eagle.Layer.get(name));

for (let layer of layers) {
  let wl = /* eagle.instances.Wire.filter(w => w.layer == layer.number)*/ layer.objects
    .filter(o => o.constructor === eagle.Wire)
    .map(w => {
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
  if(wl && wl.length > 0) lines[layer.name] = wl;
}

//console.log("lines:", Object.values(lines));

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

function readScript(path) {
  let str = fs.readFileSync(path).toString();

  let lines = str.split(/\n/g);

/*  const isCommentLine = l => {
    if("/* ".indexOf(l[0]) != -1 && l[1] == "*") return true;
    if(/^\s*\/\//.test(l) || l.startsWith("//")) return true;
    return false;
  };

  lines = lines.filter(l => !isCommentLine(l));*/
  str = lines.join("\n");

/*let i;
if((i = str.indexOf("(typeof window ==")) != -1)
  str = str.substring(0, i-3);

*/  return str;
}

var html = hu("<html>");
var head = hu("<head>", html);

hu("<script>", head).attr({
 src: `data:text/javascript;base64,${Util.base64.encode(
    readScript("utils/util.es5.js")+"\n"+   readScript("utils/dom.es5.js")+"\n"
  )}`
  });
var body = hu("<body>", html);
var svg = hu("<svg>", body).attr({
  width: "21cm",
  height: "29.7cm",
  viewBox: "0 0 210 297 "
});
var max = new Point();
var scaleFactor = 2.54 * 4;
var outerGroup = hu("<g>", svg).attr({
  transform: `scale(3.5 3.5)`
});

var grid = hu("<g>", outerGroup).attr({
  id: `Grid`,
  stroke: "#555",
  fill: "none",
  strokeWidth: 0.03,
  strokeDashoffset: "0.1",
  strokeDasharray: "0.05 0.05 0.05",
  transform: `translate(-2, 6.26) scale(10.16, 10.16) translate(0, 0)`
});

for (let i = 0; i < 30; i++) hu("<line>", grid).attr({ x1: 0, y1: i, x2: 21, y2: i });
for (let i = 0; i < 21; i++) hu("<line>", grid).attr({ x1: i, y1: 0, x2: i, y2: 29.7 });

var innerGroup = hu("<g>", outerGroup).attr({
  transform: `translate(105 148.5) scale(${scaleFactor} ${scaleFactor})`
});

hu("<rect>", outerGroup).attr({
  x: 0,
  y: 0,
  width: 210,
  height: 297,
  stroke: "red",
  strokeWidth: 0.3,
  fill: "none"
});

for (let layer in lines) {
  let l = [...lines[layer]].sort((a, b) => a.bounds().x1 - b.bounds().x1);
  let l2 = [...l].map(l => new Line({ ...l }));
  let pl = l.map(l => l.toPoints());
  let w = l.map(l => l.wire);
  let wire;
  let polylines = [];
  let newIdx;
  for(let idx = 0; idx < l.length; idx++) {
    let found = idx;
    if(l[idx] === null) continue;
    polylines.unshift(new PointList());
    if(l[idx].wire) wire = l[idx].wire;
    do {
      newIdx = found;
      const line = l[newIdx];
      found = -1;
      if(line) {
        let pts = line.toPoints();
        for(let point of pts) {
          if(max.x < point.x) max.x = point.x;
          if(max.y < point.y) max.y = point.y;
          if(polylines[0].length == 0 || !point.equal(polylines[0].last())) polylines[0].push(point);
        }
        l[newIdx] = null;
        found = findLine(l, pts, newIdx, (line, pt, ab, linei, ptidx) => {
          if(ab > 0) l[linei].swap();
        });
      }
    } while(found != -1);
  }

  const color = wire.getLayer().getColor();
  console.log("Signal: ", layer, util.inspect({ color }, { colors: true, depth: 2 }));

  function makeGroup(attrs) {
    return hu("<g>", innerGroup).attr({ transform: `scale(1,-1)  translate(${-max.x / 2} ${-max.y / 2}) `, ...attrs });
  }

  var group = makeGroup({
    id: `Wires-${layer}`,
    stroke: wire.getLayer().getColor(),
    fill: "none",
    strokeWidth: wire.width > 0 ? wire.width : 0.1,
    strokeLinecap: "round"
  });

  /*
  for(let pline of polylines) {
    var polyline = hu("<polyline>", group).attr({ points: pline.toString() });
  }*/
  l2 = l2.sort((a, b) => {
    let xdelta = a.bounds().x1 - b.bounds().x1;
    let ydelta = a.bounds().y1 - b.bounds().y1;
    return ydelta ? ydelta : xdelta;
  });

  for(let idx = 0; idx < l2.length; idx++) {
    const line = l2[idx];
    // console.log("line: ", line);
    if(line) {
      const { x1, y1, x2, y2 } = line;
      hu("<line>", group).attr({ x1, y1, x2, y2, transform: ` ` });
    }
  }
}

let viaGroup = makeGroup({
  id: `vias-pads`,
  stroke: "#008000",
  strokeWidth: 0.1,
  fill: "#008000"
});

let pkgGroup = makeGroup({
  id: `elements`,
  stroke: "#008000",
  strokeWidth: 0.1,
  fill: "#008000"
});

for (let via of eagle.instances.Via) {
  hu("<path>", viaGroup).attr({
    transform: ` translate(${via.x} ${via.y})  scale(0.5, 0.5) translate(2.5,2.5)`,
    d:
      `m-2.107 -2.107v1.2192 h1.2192  v-1.2192zm0.60994 0.25469` +
      `c 0.197  0   0.4   0.2   0.36  0.3 0  0.197 -0.158 0.36 -0.36 0.36 ` +
      `c-0.197  0  -0.4  -0.2 -0.36 -0.36 0   -0.197   0.158 -0.36 0.36 -0.36`
  });
  //  hu("<circle>", viaGroup).attr({ cx: via.x+0.5, cy: via.y+0.5, r: 0.1, fill:  '#f00', stroke: 'none', strokeWidth: 2 });
  console.log("via:", via);
}

function renderPackage(p) {
  for(let o of p.getChildren()) {
    if(o.x1 !== undefined) {
      const { x1, y1, x2, y2 } = o;
      const layer = o.getLayer();
      hu("<line>", innerGroup).attr({
        x1,
        y1,
        x2,
        y2,
        stroke: layer.getColor(),
        strokeWidth: 0.1,
        dataId: layer.number,
        dataName: layer.name
      });
    }
    console.log("o: ", o);
  }
}

let numElements = Object.values(eagle.Elements).length;
let elemIndex = -1;

let hue = 0;
let elemGroup;
var wires = [];

for (let name in eagle.Elements) {
  const e = eagle.Elements[name];
  const p = eagle.Packages[e.package];
  let rotate = /^R/.test(e.rot) ? parseInt(e.rot.substring(1)) : 0;

  let color = new dom.RGBA(hue % 360, 100, 50);
  hue += 60;
  if(p) {
    let bb = e.getBounds();
    let b = e.bbox();
    let br = e.bbox().rect;
    let c = b.center;

    elemGroup = hu("<g>", group).attr({
      id: `element-${name}`,
      transform: ` translate(${e.x}, ${e.y})  translate(1,0)  rotate(${rotate}) `
    });

    const r = e.bbox();
    const rr = r.round();

    console.log(`b ${e.name}:`, rr, `rotate: `, rotate);

    elemIndex++;

    //    hu("<rect>", elemGroup).attr({dataId: e.name, x: rr.x, y: rr.y, width: rr.width, height: rr.height, stroke: color.hex(), strokeWidth: 0.1, fill: new dom.RGBA(color.r, color.g, color.b, 180).hex(), transform: `  ` });
    hu("<circle>", elemGroup).attr({
      cy: 0,
      cx: 0,
      r: 0.5,
      fill: color.hex(),
      stroke: "#000",
      strokeWidth: 0.1,
      transform: ``
    });
    hu("<text>", elemGroup)
      .attr({
        x: 0,
        y: 0,
        fill: "#000", //color.hex(),
        stroke: "#000",
        strokeWidth: 0.5,
        textAnchor: "middle",
        textAlign: "center",
        transform: `translate(0,-0.2)  scale(0.04,-0.04)`
      })
      .text(e.name);

    for(let o of e.children()) {
      const className = Util.fnName(o.constructor);

      if(className == "Pad") {
        Point.rotate(o, (rotate * Math.PI) / 180);
        let c = [1.0563900470733643, -1.9779698848724365];
        hu("<path>", elemGroup).attr({
          dataId: e.name,
          fill: color.hex(),
          stroke: color.hex(),
          strokeWidth: "0.05",
          transform: `translate(${o.x + 0.75},${o.y - 0.25})   scale(0.5,0.5) translate(${c.join(",")}) `,
          d:
            "m-1.7561 2.79-0.35668 0.35669v0.51538l0.35668 0.35823h0.51693l0.35668-0.35823v-0.51538l-0.35668-0.35669zm0.25846 0.23779c0.20807 0 0.37736 0.1693 0.37736 0.37736 0 0.20807-0.16929 0.37736-0.37736 0.37736-0.20806 0-0.37736-0.16929-0.37736-0.37736 0-0.20806 0.1693-0.37736 0.37736-0.37736z"
        });
      } else if(className == "Wire") {
        let { x1, y1, x2, y2 } = o;
        let layer = eagle.Layer.get(o.layer);
        /*
        x1 -= e.x;
        x2 -= e.x;
        y1 -= e.y;
        y2 -= e.y;*/
        x1 += 0.5;
        x2 += 0.5;
        y1 += 0.875;
        y2 += 0.875;

        let color = layer.getColor();
        wires.push({ x1, y1, x2, y2, stroke: color, strokeWidth: "0.1" });
        /*

          let s= hu("<line>", elemGroup).attr({
          dataId: e.name,
          fill: 'none',
          stroke: color,
          strokeWidth: "0.05",
           x1, y1, x2, y2
        }).n;

        console.log("wire: ", util.inspect({ out:s.outerHTML }, { depth: 0, colors: true }));
*/
      }
    }
    console.log("bb:", bb);
  }
}

group = makeGroup({ id: "all-wires" });

wires.forEach(w => {
  let { x1, y1, x2, y2, stroke, strokeWidth } = w;
  let s = hu("<line>", group).attr(w).n;
  console.log("wire: ", util.inspect({ out: s.outerHTML }, { depth: 0, colors: true }));
});

console.log("max:", max);

let xmlData =
  `<?xml version="1.0" encoding="utf-8"?>\n` +
  html.n.outerHTML.replace(/<svg /, '<svg xmlns="http://www.w3.org/2000/svg" ').replace(/><([^/])/g, ">\n<$1");

fs.writeFileSync("out.html", xmlData);

/*
let wires = eagle.Wires.map(w => { let wire = new dom.Line(w); return Object.assign(wire, w);   });
*/

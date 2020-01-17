const DOMParser = require("xmldom").DOMParser;
const fs = require("fs");
const util = require("util");
const Util = require("./utils/util.js");
const dom = require("./utils/dom.es5.js");
const eagle = require("./utils/eagle.js");

var data = fs.readFileSync("/home/roman/Dokumente/Sources/pictest/eagle/PIC18F2550-USB+ICSP-Board.brd").toString();
var xml = new DOMParser().parseFromString(data);
var doc = xml.documentElement;

global.document = doc;
console.log(eagle);

function xml2obj(node) {
  let a =
    node.attributes && node.attributes.length > 0
      ? Array.from(node.attributes).reduce(
          (acc, attr) => ({ ...acc, [attr.name]: isNaN(parseFloat(attr.value)) ? attr.value : parseFloat(attr.value) }),
          {}
        )
      : {};
  var obj;
  //() console.log("Node:", Object.keys(node), `data:"${node.data}"`);

  if(node.tagName !== undefined) {
    a.tag = node.tagName;
    obj = eagle.default(a);
    if(obj !== null) a = obj;
  } else return null;
  //
  var children = [];

  //  console.log("obj: ", obj);

  if(node.childNodes !== undefined && node.childNodes !== null) {
    for(let child of Array.from(node.childNodes)) {
      const obj = xml2obj(child);
      if(obj !== null) {
        //  obj.parent = a;
        children.push(obj);
      }
    }
  }

  if(children.length) a.children = children;

  return a;
}

var out = xml2obj(doc);
//console.log("obj: ", util.inspect(out, {colors: true, depth: 10 }));

console.log("eagle.Elements: ", Object.keys(eagle.instances), eagle.instances.Element);

/*
Tree.walk(doc, function(node) {
  if(node.attributes && node.attributes.length > 0) {
    const a = Array.from(node.attributes).reduce(
      (acc, attr) => ({ ...acc, [attr.name]: isNaN(parseFloat(attr.value)) ? attr.value : parseFloat(attr.value) }),
      {}
    );

    node.o = Element.toObject(node);

    if(node.childNodes) node.o = { ...node.o, children: [] };

    if(typeof node.parentNode.o != "object") node.parentNode.o = {};
    if(typeof node.parentNode.o.children != "object") node.parentNode.o.children = [];

    node.parentNode.o.children.push(node.o);

    console.log("attributes: ", a);
    console.log("xpath: ", Object.keys(node));
  }
});*/

console.log("Finished!");

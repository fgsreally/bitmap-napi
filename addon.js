var addon = require('bindings')('addon');


var obj=new addon.Bitset(5);
console.log(obj.getPos(3))
obj.setPos(2)
console.log(obj.getPos(3))
obj.cancelPos(2)
console.log(obj.getPos(3))
var arr=obj.value()
console.log(arr)





var CreateModuleInstance = (function() {
  var _scriptDir = typeof document !== 'undefined' && document.currentScript ? document.currentScript.src : undefined;
  if (typeof __filename !== 'undefined') _scriptDir = _scriptDir || __filename;
  return (
function(CreateModuleInstance) {
  CreateModuleInstance = CreateModuleInstance || {};



  return CreateModuleInstance.ready
}
);
})();
if (typeof exports === 'object' && typeof module === 'object')
  module.exports = CreateModuleInstance;
else if (typeof define === 'function' && define['amd'])
  define([], function() { return CreateModuleInstance; });
else if (typeof exports === 'object')
  exports["CreateModuleInstance"] = CreateModuleInstance;
window.CreateModuleInstance({noInitialRun:!1}).then(function(a){window.Main={Module:a};document.getElementById("txtResult").innerHTML+="InstanceCreated at "+Date()+"<br />"});

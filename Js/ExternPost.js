// window.Main = {};
// // function FunctionExists(name) {
// //     return (typeof window[name] === 'function');
// // };

// // document.getElementById("txtResult").innerHTML += "SharedArrayBuffer: " +String(typeof SharedArrayBuffer) + "<br />"
// // document.getElementById("txtResult").innerHTML += "Initializing <br />"
// // try {
// //     if (FunctionExists("CreateModuleInstance")) {
// //         document.getElementById("txtResult").innerHTML += "Function exists <br />";
// //     }
// //     else {
// //         document.getElementById("txtResult").innerHTML += "Funtion don't exists <br />";
// //     }
//     CreateModuleInstance()
//         .then(instance => {
//             window.Main.Module = instance;
//             // document.getElementById("txtResult").innerHTML += "InstanceCreated <br />";
//         });
// // } catch (error) {
// //     document.getElementById("txtResult").innerHTML += String(error) + " <br />"
// // }

// // console.log("Hello");

// // console.log("Loaded Script");

window.CreateModuleInstance({ noInitialRun: false })
    .then(instance => {
        window.Main = { Module: instance };
        document.getElementById("txtResult").innerHTML += "InstanceCreated at "+ Date() + "<br />";
    });
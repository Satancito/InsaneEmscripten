window.CreateModuleInstance({ noInitialRun: false })
    .then(instance => {
        window.Insane = { Module: instance };
    });
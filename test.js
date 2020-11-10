const myPromisify = (fn) => {
  return (...args) => {
    
    return new Promise((resolve, reject) => {
      function customCallback(err, ...results) {
        if (err) {
          return reject(err)
        }
        return resolve(results.length === 1 ? results[0] : results) 
       }
       args.push(customCallback)
       fn.call(this, ...args)
     });


  };

  
};

mu


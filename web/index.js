import PolyModule from './poly_wasm.js';


// Then here we are prolly gonna grab the html buttons and add event listeners to build into arrays and pass into the wasm object builder then do the computation right


PolyModule().then(Module => {
  const add = Module.cwrap('wasm_add_double', 'number', ['number', 'number']);

  const a = 2.5;
  const b = 3.5;
  const result = add(a, b);

  console.log(`${a} + ${b} = ${result}`);
  const out = document.getElementById('output');
  if (out) {
    out.textContent = `${a} + ${b} = ${result}`;
  }
});

// wasm-loader.js
export async function loadWasmModule(url) {
    const response = await fetch(url);
    const buffer = await response.arrayBuffer();
    const module = await WebAssembly.compile(buffer);
    const instance = await WebAssembly.instantiate(module);
    return instance.exports;
  }
  
  // Example usage in ComplexPolynomialCalculator.vue
  import { loadWasmModule } from './wasm-loader';
  
  export default {
    async mounted() {
      const wasmModule = await loadWasmModule('path_to_your_wasm_file.wasm');
      // Now you can use your wasm functions
      const result = wasmModule.someFunction(); // Example function call
    },
  };
  
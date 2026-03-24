import fs from 'node:fs'
import path from 'node:path'
import { fileURLToPath } from 'node:url'

const __filename = fileURLToPath(import.meta.url)
const __dirname = path.dirname(__filename)

const wasmPath = path.join(__dirname, '..', 'web', 'public', 'poly.wasm')
const bytes = fs.readFileSync(wasmPath)
const mod = await WebAssembly.compile(bytes)

console.log(`WASM: ${wasmPath}`)
console.log(WebAssembly.Module.exports(mod))


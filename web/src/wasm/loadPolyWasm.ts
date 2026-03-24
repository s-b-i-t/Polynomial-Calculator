export type PolyWasmExports = {
  add: (a: number, b: number) => number
}

type RawPolyWasmExports = WebAssembly.Exports & Partial<Record<'add' | '_add', unknown>>

function normalizeExports(exports: RawPolyWasmExports): PolyWasmExports {
  const add = (exports.add ?? exports._add) as unknown
  if (typeof add !== 'function') {
    throw new Error('WASM module missing export: add (or _add)')
  }
  return { add: add as (a: number, b: number) => number }
}

export async function loadPolyWasm(url: string): Promise<PolyWasmExports> {
  const imports: WebAssembly.Imports = {}

  const instantiate = async () => {
    if ('instantiateStreaming' in WebAssembly) {
      try {
        const res = await WebAssembly.instantiateStreaming(fetch(url), imports)
        return res.instance.exports
      } catch {
        // Some servers don't serve application/wasm; fall back to ArrayBuffer path.
      }
    }

    const response = await fetch(url)
    if (!response.ok) {
      throw new Error(`Failed to fetch WASM (${response.status}): ${response.statusText}`)
    }
    const bytes = await response.arrayBuffer()
    const res = await WebAssembly.instantiate(bytes, imports)
    return res.instance.exports
  }

  const exports = await instantiate()
  return normalizeExports(exports as RawPolyWasmExports)
}

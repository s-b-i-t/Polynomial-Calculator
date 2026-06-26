type RawPolyWasmModule = {
  cwrap: (
    name: string,
    returnType: 'number' | null,
    argTypes: Array<'string' | 'number'>,
  ) => (...args: Array<string | number>) => number | null
  UTF8ToString: (ptr: number) => string
}

export type PolyWasmEngine = {
  add: (p: string, q: string) => string
  subtract: (p: string, q: string) => string
  multiply: (p: string, q: string) => string
  divide: (p: string, q: string) => string
  roots: (p: string) => string
}

declare global {
  interface Window {
    createPolyWasmModule?: (options?: {
      locateFile?: (path: string) => string
    }) => Promise<RawPolyWasmModule>
  }
}

let loadPromise: Promise<PolyWasmEngine> | null = null

function loadScript(src: string): Promise<void> {
  const existing = document.querySelector<HTMLScriptElement>(`script[src="${src}"]`)
  if (existing) {
    return Promise.resolve()
  }

  return new Promise((resolve, reject) => {
    const script = document.createElement('script')
    script.src = src
    script.async = true
    script.onload = () => resolve()
    script.onerror = () => reject(new Error(`Unable to load ${src}`))
    document.head.appendChild(script)
  })
}

export function loadPolyWasmEngine(): Promise<PolyWasmEngine> {
  if (loadPromise) {
    return loadPromise
  }

  loadPromise = loadScript('/poly_wasm.js').then(async () => {
    if (!window.createPolyWasmModule) {
      throw new Error('poly_wasm.js loaded, but createPolyWasmModule was not registered.')
    }

    const module = await window.createPolyWasmModule({
      locateFile: (path) => `/${path}`,
    })

    const free = module.cwrap('poly_free', null, ['number'])

    function wrapStringResult<Args extends Array<string | number>>(
      name: string,
      argTypes: Array<'string' | 'number'>,
    ) {
      const fn = module.cwrap(name, 'number', argTypes)
      return (...args: Args) => {
        const ptr = fn(...args)
        if (typeof ptr !== 'number' || ptr === 0) {
          throw new Error(`${name} returned an empty pointer.`)
        }

        try {
          return module.UTF8ToString(ptr)
        } finally {
          free(ptr)
        }
      }
    }

    return {
      add: wrapStringResult<[string, string]>('poly_add', ['string', 'string']),
      subtract: wrapStringResult<[string, string]>('poly_subtract', ['string', 'string']),
      multiply: wrapStringResult<[string, string]>('poly_multiply', ['string', 'string']),
      divide: wrapStringResult<[string, string]>('poly_divide', ['string', 'string']),
      roots: wrapStringResult<[string]>('poly_roots', ['string']),
    }
  })

  return loadPromise
}

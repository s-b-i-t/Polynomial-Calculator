import { useEffect, useMemo, useState } from 'react'
import { loadPolyWasm, type PolyWasmExports } from './wasm/loadPolyWasm'

type Mode = 'Basic' | 'Complex' | 'Polynomial'

type MemorySlot = {
  name: string
  value: string
}

type Key =
  | { kind: 'input'; label: string; value: string }
  | { kind: 'action'; label: string; action: 'clear' | 'backspace' | 'enter' }
  | { kind: 'modeAction'; label: string; action: string }

function ButtonGrid({
  title,
  keys,
  onKey,
}: {
  title: string
  keys: Key[]
  onKey: (key: Key) => void
}) {
  return (
    <section style={{ display: 'flex', flexDirection: 'column', gap: 8 }}>
      <div style={{ fontWeight: 600 }}>{title}</div>
      <div
        style={{
          display: 'grid',
          gridTemplateColumns: 'repeat(6, minmax(0, 1fr))',
          gap: 6,
        }}
      >
        {keys.map((key, index) => (
          <button
            key={`${key.kind}-${key.label}-${index}`}
            type="button"
            onClick={() => onKey(key)}
            style={{ padding: 10 }}
          >
            {key.label}
          </button>
        ))}
      </div>
    </section>
  )
}

export default function App() {
  const [mode, setMode] = useState<Mode>('Polynomial')
  const [expression, setExpression] = useState('')
  const [result, setResult] = useState<string>('')
  const [history, setHistory] = useState<string[]>([])
  const [memory, setMemory] = useState<MemorySlot[]>([
    { name: 'Ans', value: '' },
    { name: 'A', value: '' },
    { name: 'B', value: '' },
    { name: 'C', value: '' },
  ])

  const [wasm, setWasm] = useState<PolyWasmExports | null>(null)
  const [wasmStatus, setWasmStatus] = useState<'idle' | 'loading' | 'ready' | 'error'>('idle')
  const [wasmError, setWasmError] = useState<string>('')

  useEffect(() => {
    let cancelled = false
    setWasmStatus('loading')
    setWasmError('')
    loadPolyWasm('/poly.wasm')
      .then((exports) => {
        if (cancelled) return
        setWasm(exports)
        setWasmStatus('ready')
      })
      .catch((err: unknown) => {
        if (cancelled) return
        setWasm(null)
        setWasmStatus('error')
        setWasmError(err instanceof Error ? err.message : String(err))
      })
    return () => {
      cancelled = true
    }
  }, [])

  const topKeys = useMemo<Key[]>(
    () => [
      { kind: 'action', label: 'Clear', action: 'clear' },
      { kind: 'action', label: '⌫', action: 'backspace' },
      { kind: 'action', label: '=', action: 'enter' },
      { kind: 'modeAction', label: 'Store → A', action: 'storeA' },
      { kind: 'modeAction', label: 'Recall A', action: 'recallA' },
      { kind: 'modeAction', label: 'Use Ans', action: 'useAns' },
    ],
    [],
  )

  const keypad = useMemo<Key[]>(
    () => [
      { kind: 'input', label: '7', value: '7' },
      { kind: 'input', label: '8', value: '8' },
      { kind: 'input', label: '9', value: '9' },
      { kind: 'input', label: '(', value: '(' },
      { kind: 'input', label: ')', value: ')' },
      { kind: 'input', label: '÷', value: '/' },
      { kind: 'input', label: '4', value: '4' },
      { kind: 'input', label: '5', value: '5' },
      { kind: 'input', label: '6', value: '6' },
      { kind: 'input', label: 'x', value: 'x' },
      { kind: 'input', label: '^', value: '^' },
      { kind: 'input', label: '×', value: '*' },
      { kind: 'input', label: '1', value: '1' },
      { kind: 'input', label: '2', value: '2' },
      { kind: 'input', label: '3', value: '3' },
      { kind: 'input', label: '+', value: '+' },
      { kind: 'input', label: '-', value: '-' },
      { kind: 'input', label: '%', value: '%' },
      { kind: 'input', label: '0', value: '0' },
      { kind: 'input', label: '.', value: '.' },
      { kind: 'input', label: ',', value: ',' },
      { kind: 'input', label: ' ', value: ' ' },
      { kind: 'input', label: 'i', value: 'i' },
      { kind: 'input', label: 'π', value: 'pi' },
    ],
    [],
  )

  const modeKeys = useMemo<Key[]>(() => {
    if (mode === 'Basic') {
      return [
        { kind: 'modeAction', label: 'sin', action: 'sin' },
        { kind: 'modeAction', label: 'cos', action: 'cos' },
        { kind: 'modeAction', label: 'tan', action: 'tan' },
        { kind: 'modeAction', label: 'log', action: 'log' },
        { kind: 'modeAction', label: 'ln', action: 'ln' },
        { kind: 'modeAction', label: 'sqrt', action: 'sqrt' },
      ]
    }
    if (mode === 'Complex') {
      return [
        { kind: 'modeAction', label: 'conj', action: 'conj' },
        { kind: 'modeAction', label: 'abs', action: 'abs' },
        { kind: 'modeAction', label: 'arg', action: 'arg' },
        { kind: 'modeAction', label: 'polar', action: 'polar' },
        { kind: 'modeAction', label: 'rect', action: 'rect' },
        { kind: 'modeAction', label: 're/im', action: 'reim' },
      ]
    }
    return [
      { kind: 'modeAction', label: 'eval', action: 'eval' },
      { kind: 'modeAction', label: 'derive', action: 'derive' },
      { kind: 'modeAction', label: 'integrate', action: 'integrate' },
      { kind: 'modeAction', label: 'roots', action: 'roots' },
      { kind: 'modeAction', label: 'coeffs', action: 'coeffs' },
      { kind: 'modeAction', label: 'expand', action: 'expand' },
    ]
  }, [mode])

  function updateMemory(slotName: string, value: string) {
    setMemory((prev) => prev.map((s) => (s.name === slotName ? { ...s, value } : s)))
  }

  function getMemory(slotName: string) {
    return memory.find((s) => s.name === slotName)?.value ?? ''
  }

  function onKey(key: Key) {
    if (key.kind === 'input') {
      setExpression((prev) => prev + key.value)
      return
    }

    if (key.kind === 'action') {
      if (key.action === 'clear') {
        setExpression('')
        setResult('')
        return
      }
      if (key.action === 'backspace') {
        setExpression((prev) => prev.slice(0, -1))
        return
      }
      if (key.action === 'enter') {
        setHistory((prev) => [expression, ...prev].slice(0, 12))
        updateMemory('Ans', result || expression)
        return
      }
      return
    }

    if (key.kind === 'modeAction') {
      if (key.action === 'storeA') {
        updateMemory('A', result || expression)
        return
      }
      if (key.action === 'recallA') {
        setExpression((prev) => prev + getMemory('A'))
        return
      }
      if (key.action === 'useAns') {
        setExpression((prev) => prev + getMemory('Ans'))
        return
      }

      setExpression((prev) => prev + `${key.label}(`)
      return
    }
  }

  async function runWasmSmokeTest() {
    if (!wasm) {
      setResult('WASM not ready yet')
      return
    }
    try {
      const computed = wasm.add(2, 3)
      setResult(`WASM add(2, 3) = ${computed}`)
    } catch (err: unknown) {
      setResult(err instanceof Error ? err.message : String(err))
    }
  }

  return (
    <div style={{ display: 'flex', flexDirection: 'column', gap: 12, padding: 12 }}>
      <header style={{ display: 'flex', alignItems: 'center', gap: 12, flexWrap: 'wrap' }}>
        <div style={{ fontWeight: 700 }}>Polynomial Calculator (React + WASM)</div>
        <div style={{ display: 'flex', gap: 6, alignItems: 'center' }}>
          <span>Mode:</span>
          {(['Basic', 'Complex', 'Polynomial'] as const).map((m) => (
            <button
              key={m}
              type="button"
              onClick={() => setMode(m)}
              style={{ padding: '6px 10px' }}
            >
              {m}
            </button>
          ))}
        </div>
        <div style={{ marginLeft: 'auto', display: 'flex', gap: 8, alignItems: 'center' }}>
          <span>WASM:</span>
          <span>
            {wasmStatus === 'ready' ? 'ready' : wasmStatus === 'loading' ? 'loading…' : wasmStatus}
          </span>
          <button type="button" onClick={runWasmSmokeTest} style={{ padding: '6px 10px' }}>
            Run WASM smoke test
          </button>
        </div>
      </header>

      {wasmStatus === 'error' ? (
        <div style={{ border: '1px solid #ccc', padding: 10 }}>
          <div style={{ fontWeight: 600 }}>WASM load error</div>
          <div style={{ whiteSpace: 'pre-wrap' }}>{wasmError}</div>
          <div style={{ marginTop: 8 }}>
            Once we build a `poly.wasm`, we’ll serve it from `web/public/poly.wasm`.
          </div>
        </div>
      ) : null}

      <div style={{ display: 'grid', gridTemplateColumns: '320px 1fr 320px', gap: 12 }}>
        <aside style={{ border: '1px solid #ccc', padding: 10, display: 'flex', flexDirection: 'column', gap: 10 }}>
          <div style={{ fontWeight: 600 }}>Values / Memory</div>
          <div style={{ display: 'flex', flexDirection: 'column', gap: 8 }}>
            {memory.map((slot) => (
              <div key={slot.name} style={{ display: 'flex', gap: 8, alignItems: 'center' }}>
                <div style={{ width: 40 }}>{slot.name}</div>
                <input
                  value={slot.value}
                  onChange={(e) => updateMemory(slot.name, e.target.value)}
                  placeholder={`${slot.name} value`}
                  style={{ flex: 1, padding: 6 }}
                />
              </div>
            ))}
          </div>
        </aside>

        <main style={{ border: '1px solid #ccc', padding: 10, display: 'flex', flexDirection: 'column', gap: 10 }}>
          <div style={{ fontWeight: 600 }}>Workspace</div>
          <div style={{ display: 'flex', flexDirection: 'column', gap: 8 }}>
            <label style={{ display: 'flex', flexDirection: 'column', gap: 4 }}>
              <span>Expression</span>
              <input
                value={expression}
                onChange={(e) => setExpression(e.target.value)}
                placeholder={mode === 'Polynomial' ? 'e.g. 1 + 2x + 3x^2' : 'Type an expression'}
                style={{ padding: 8 }}
              />
            </label>

            <label style={{ display: 'flex', flexDirection: 'column', gap: 4 }}>
              <span>Result</span>
              <input value={result} readOnly placeholder="Result output" style={{ padding: 8 }} />
            </label>
          </div>

          <ButtonGrid title="Quick Actions" keys={topKeys} onKey={onKey} />
          <ButtonGrid title={`${mode} Functions`} keys={modeKeys} onKey={onKey} />
          <ButtonGrid title="Keypad" keys={keypad} onKey={onKey} />
        </main>

        <aside style={{ border: '1px solid #ccc', padding: 10, display: 'flex', flexDirection: 'column', gap: 10 }}>
          <div style={{ fontWeight: 600 }}>Context</div>
          <div>Mode-specific helpers will live here.</div>
          <div style={{ border: '1px dashed #ccc', padding: 10 }}>
            {mode === 'Polynomial' ? (
              <div>
                <div style={{ fontWeight: 600 }}>Polynomial panel (placeholder)</div>
                <ul style={{ margin: 0, paddingLeft: 18 }}>
                  <li>Coefficients table</li>
                  <li>Degree / leading term</li>
                  <li>Graph preview</li>
                </ul>
              </div>
            ) : mode === 'Complex' ? (
              <div>
                <div style={{ fontWeight: 600 }}>Complex panel (placeholder)</div>
                <ul style={{ margin: 0, paddingLeft: 18 }}>
                  <li>Rectangular / polar views</li>
                  <li>Conjugate / magnitude</li>
                </ul>
              </div>
            ) : (
              <div>
                <div style={{ fontWeight: 600 }}>Basic panel (placeholder)</div>
                <ul style={{ margin: 0, paddingLeft: 18 }}>
                  <li>Constants</li>
                  <li>Functions reference</li>
                </ul>
              </div>
            )}
          </div>

          <div style={{ fontWeight: 600 }}>History</div>
          <div style={{ display: 'flex', flexDirection: 'column', gap: 6 }}>
            {history.length === 0 ? <div>(empty)</div> : null}
            {history.map((item, index) => (
              <button
                key={`${item}-${index}`}
                type="button"
                onClick={() => setExpression(item)}
                style={{ textAlign: 'left', padding: 8 }}
              >
                {item}
              </button>
            ))}
          </div>
        </aside>
      </div>
    </div>
  )
}


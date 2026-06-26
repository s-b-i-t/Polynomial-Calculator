import { useEffect, useState, type Dispatch, type SetStateAction } from 'react'
import './App.css'
import { loadPolyWasmEngine, type PolyWasmEngine } from './wasm/polyWasm'

type Polynomial = number[]
type WasmStatus = 'loading' | 'ready' | 'error'

function exponentLabel(degree: number): string {
  if (degree === 0) return ''
  if (degree === 1) return 'x'
  if (degree === 2) return 'x²'
  if (degree === 3) return 'x³'
  return `x^${degree}`
}

function serializePolynomial(coefficients: Polynomial): string {
  return coefficients.map((value) => (Number.isFinite(value) ? value : 0)).join(',')
}

function displayPolynomial(coefficients: Polynomial): string {
  const terms = coefficients.map((coefficient, degree) => {
    const suffix = exponentLabel(degree)
    return `${coefficient}${suffix ? suffix : ''}`
  })
  return terms.join(' + ')
}

export default function App() {
  const [pCoefficients, setPCoefficients] = useState<Polynomial>([1, 0, 1])
  const [qCoefficients, setQCoefficients] = useState<Polynomial>([1, -1])
  const [result, setResult] = useState('Choose an operation.')
  const [detail, setDetail] = useState('Results are computed by the C++ WASM engine.')
  const [error, setError] = useState('')
  const [wasmEngine, setWasmEngine] = useState<PolyWasmEngine | null>(null)
  const [wasmStatus, setWasmStatus] = useState<WasmStatus>('loading')
  const [wasmError, setWasmError] = useState('')

  useEffect(() => {
    let cancelled = false

    loadPolyWasmEngine()
      .then((engine) => {
        if (cancelled) return
        setWasmEngine(engine)
        setWasmStatus('ready')
        setWasmError('')
      })
      .catch((loadError: unknown) => {
        if (cancelled) return
        setWasmEngine(null)
        setWasmStatus('error')
        setWasmError(loadError instanceof Error ? loadError.message : String(loadError))
      })

    return () => {
      cancelled = true
    }
  }, [])

  function updateCoefficient(
    setter: Dispatch<SetStateAction<Polynomial>>,
    degree: number,
    value: number,
  ) {
    setter((current) => current.map((coefficient, index) => (index === degree ? value : coefficient)))
  }

  function addTerm(setter: Dispatch<SetStateAction<Polynomial>>) {
    setter((current) => [...current, 0])
  }

  function removeTerm(setter: Dispatch<SetStateAction<Polynomial>>) {
    setter((current) => (current.length > 1 ? current.slice(0, -1) : [0]))
  }

  function clearPolynomial(setter: Dispatch<SetStateAction<Polynomial>>) {
    setter([0])
  }

  function handleEngineResult(nextResult: string, nextDetail: string) {
    setResult(nextResult)
    setDetail(nextDetail)
    setError(nextResult.startsWith('Error:') ? nextResult : '')
  }

  function requireEngine(): PolyWasmEngine | null {
    if (wasmEngine) {
      return wasmEngine
    }

    const message =
      wasmStatus === 'error'
        ? `WASM failed to load: ${wasmError}`
        : 'WASM engine is still loading.'
    setError(message)
    setDetail('No TypeScript fallback is available.')
    return null
  }

  function runBinary(label: string, operation: (engine: PolyWasmEngine, p: string, q: string) => string) {
    const engine = requireEngine()
    if (!engine) return

    const p = serializePolynomial(pCoefficients)
    const q = serializePolynomial(qCoefficients)
    handleEngineResult(operation(engine, p, q), `WASM: ${label}`)
  }

  function runRoots() {
    const engine = requireEngine()
    if (!engine) return

    handleEngineResult(engine.roots(serializePolynomial(pCoefficients)), 'WASM: roots of P')
  }

  function renderBuilder(
    label: 'P' | 'Q',
    coefficients: Polynomial,
    setter: Dispatch<SetStateAction<Polynomial>>,
  ) {
    return (
      <section className="builder" aria-label={`${label} polynomial builder`}>
        <div className="builder-header">
          <div>
            <span>{label}(x)</span>
            <strong>{displayPolynomial(coefficients)}</strong>
          </div>
          <div className="builder-actions">
            <button type="button" onClick={() => addTerm(setter)}>
              Add term to {label}
            </button>
            <button type="button" onClick={() => removeTerm(setter)}>
              Remove term from {label}
            </button>
            <button type="button" onClick={() => clearPolynomial(setter)}>
              Clear {label}
            </button>
          </div>
        </div>

        <div className="terms">
          {coefficients.map((coefficient, degree) => (
            <div className="term-control" key={`${label}-${degree}`}>
              <span className="term-label">{degree === 0 ? 'constant' : exponentLabel(degree)}</span>
              <button
                type="button"
                aria-label={`Decrease ${label} ${exponentLabel(degree) || 'constant'} coefficient`}
                onClick={() => updateCoefficient(setter, degree, coefficient - 1)}
              >
                -
              </button>
              <output aria-label={`${label} coefficient for degree ${degree}`}>{coefficient}</output>
              <button
                type="button"
                aria-label={`Increase ${label} ${exponentLabel(degree) || 'constant'} coefficient`}
                onClick={() => updateCoefficient(setter, degree, coefficient + 1)}
              >
                +
              </button>
              <button
                type="button"
                className="secondary"
                onClick={() => updateCoefficient(setter, degree, 0)}
              >
                Clear
              </button>
            </div>
          ))}
        </div>
      </section>
    )
  }

  const isReady = wasmStatus === 'ready'

  return (
    <main className="app-shell">
      <section className="workspace" aria-labelledby="app-title">
        <header className="workspace-header">
          <div>
            <p className="eyebrow">C++ WebAssembly engine</p>
            <h1 id="app-title">Polynomial Calculator</h1>
          </div>
          <div className={`engine-status ${wasmStatus}`}>
            <span>WASM engine: {wasmStatus}</span>
            {wasmStatus === 'error' ? <small>{wasmError}</small> : null}
          </div>
        </header>

        <div className="builders">
          {renderBuilder('P', pCoefficients, setPCoefficients)}
          {renderBuilder('Q', qCoefficients, setQCoefficients)}
        </div>

        <section className="operations" aria-label="Polynomial operations">
          <button type="button" disabled={!isReady} onClick={() => runBinary('P + Q', (engine, p, q) => engine.add(p, q))}>
            P + Q
          </button>
          <button type="button" disabled={!isReady} onClick={() => runBinary('P - Q', (engine, p, q) => engine.subtract(p, q))}>
            P - Q
          </button>
          <button type="button" disabled={!isReady} onClick={() => runBinary('P × Q', (engine, p, q) => engine.multiply(p, q))}>
            P × Q
          </button>
          <button type="button" disabled={!isReady} onClick={() => runBinary('P ÷ Q', (engine, p, q) => engine.divide(p, q))}>
            P ÷ Q
          </button>
          <button type="button" disabled={!isReady} onClick={runRoots}>
            Roots of P
          </button>
        </section>

        <section className="result-panel" aria-live="polite" aria-label="Result">
          <div>
            <span>{detail}</span>
            <output>{result}</output>
          </div>
          {error ? <p className="error">{error}</p> : null}
        </section>
      </section>
    </main>
  )
}

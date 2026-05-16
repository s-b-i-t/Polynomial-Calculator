import { useEffect, useMemo, useState } from 'react'
import './App.css'
import { loadPolyWasmEngine, type PolyWasmEngine } from './wasm/polyWasm'

type Polynomial = number[]

type RationalPolynomial = {
  numerator: Polynomial
  denominator: Polynomial
}

type BinaryOperation = '+' | '-' | '*' | '/'
type WasmStatus = 'loading' | 'ready' | 'error'

const EPSILON = 1e-10

const examples = [
  { label: 'Quadratic plus line', p: '1, 2, 3', q: '4, -1' },
  { label: 'Difference of squares', p: '-1, 0, 1', q: '1, 1' },
  { label: 'Rational result', p: '2, 0, 1', q: '1, -1' },
]

function trimPolynomial(poly: Polynomial): Polynomial {
  const trimmed = [...poly]
  while (trimmed.length > 1 && Math.abs(trimmed[trimmed.length - 1]) < EPSILON) {
    trimmed.pop()
  }
  return trimmed.length === 0 ? [0] : trimmed.map((value) => (Math.abs(value) < EPSILON ? 0 : value))
}

function isZeroPolynomial(poly: Polynomial): boolean {
  return trimPolynomial(poly).every((value) => Math.abs(value) < EPSILON)
}

function parsePolynomial(input: string): Polynomial {
  const tokens = input
    .split(/[,\n]+/)
    .map((token) => token.trim())
    .filter(Boolean)

  if (tokens.length === 0) {
    throw new Error('Enter at least one coefficient.')
  }

  const coefficients = tokens.map((token) => {
    const value = Number(token)
    if (!Number.isFinite(value)) {
      throw new Error(`Invalid coefficient: ${token}`)
    }
    return value
  })

  return trimPolynomial(coefficients)
}

function addPolynomials(a: Polynomial, b: Polynomial): Polynomial {
  const length = Math.max(a.length, b.length)
  const result = Array.from({ length }, (_, index) => (a[index] ?? 0) + (b[index] ?? 0))
  return trimPolynomial(result)
}

function subtractPolynomials(a: Polynomial, b: Polynomial): Polynomial {
  const length = Math.max(a.length, b.length)
  const result = Array.from({ length }, (_, index) => (a[index] ?? 0) - (b[index] ?? 0))
  return trimPolynomial(result)
}

function multiplyPolynomials(a: Polynomial, b: Polynomial): Polynomial {
  if (isZeroPolynomial(a) || isZeroPolynomial(b)) {
    return [0]
  }

  const result = Array.from({ length: a.length + b.length - 1 }, () => 0)
  a.forEach((aCoeff, aIndex) => {
    b.forEach((bCoeff, bIndex) => {
      result[aIndex + bIndex] += aCoeff * bCoeff
    })
  })
  return trimPolynomial(result)
}

function dividePolynomials(dividend: Polynomial, divisor: Polynomial): {
  quotient: Polynomial
  remainder: Polynomial
} {
  const cleanDividend = trimPolynomial(dividend)
  const cleanDivisor = trimPolynomial(divisor)

  if (isZeroPolynomial(cleanDivisor)) {
    throw new Error('Division by the zero polynomial is undefined.')
  }

  const dividendDegree = cleanDividend.length - 1
  const divisorDegree = cleanDivisor.length - 1

  if (dividendDegree < divisorDegree) {
    return { quotient: [0], remainder: cleanDividend }
  }

  const quotient = Array.from({ length: dividendDegree - divisorDegree + 1 }, () => 0)
  const remainder = [...cleanDividend]
  const leadingDivisorCoeff = cleanDivisor[divisorDegree]

  for (let degree = dividendDegree; degree >= divisorDegree; degree -= 1) {
    const factor = remainder[degree] / leadingDivisorCoeff
    const quotientIndex = degree - divisorDegree
    quotient[quotientIndex] = factor

    for (let divisorIndex = 0; divisorIndex <= divisorDegree; divisorIndex += 1) {
      remainder[quotientIndex + divisorIndex] -= factor * cleanDivisor[divisorIndex]
    }
  }

  return {
    quotient: trimPolynomial(quotient),
    remainder: trimPolynomial(remainder.slice(0, divisorDegree)),
  }
}

function derivative(poly: Polynomial): Polynomial {
  if (poly.length <= 1) {
    return [0]
  }
  return trimPolynomial(poly.slice(1).map((coefficient, index) => coefficient * (index + 1)))
}

function evaluate(poly: Polynomial, xValue: number): number {
  return poly.reduceRight((accumulator, coefficient) => accumulator * xValue + coefficient, 0)
}

function makeRational(poly: Polynomial): RationalPolynomial {
  return { numerator: trimPolynomial(poly), denominator: [1] }
}

function addRationals(a: RationalPolynomial, b: RationalPolynomial): RationalPolynomial {
  return {
    numerator: addPolynomials(
      multiplyPolynomials(a.numerator, b.denominator),
      multiplyPolynomials(b.numerator, a.denominator),
    ),
    denominator: multiplyPolynomials(a.denominator, b.denominator),
  }
}

function subtractRationals(a: RationalPolynomial, b: RationalPolynomial): RationalPolynomial {
  return {
    numerator: subtractPolynomials(
      multiplyPolynomials(a.numerator, b.denominator),
      multiplyPolynomials(b.numerator, a.denominator),
    ),
    denominator: multiplyPolynomials(a.denominator, b.denominator),
  }
}

function multiplyRationals(a: RationalPolynomial, b: RationalPolynomial): RationalPolynomial {
  return {
    numerator: multiplyPolynomials(a.numerator, b.numerator),
    denominator: multiplyPolynomials(a.denominator, b.denominator),
  }
}

function divideRationals(a: RationalPolynomial, b: RationalPolynomial): RationalPolynomial {
  if (isZeroPolynomial(b.numerator)) {
    throw new Error('Division by the zero polynomial is undefined.')
  }

  return {
    numerator: multiplyPolynomials(a.numerator, b.denominator),
    denominator: multiplyPolynomials(a.denominator, b.numerator),
  }
}

function applyOperation(operation: BinaryOperation, p: Polynomial, q: Polynomial): RationalPolynomial {
  const left = makeRational(p)
  const right = makeRational(q)

  if (operation === '+') return addRationals(left, right)
  if (operation === '-') return subtractRationals(left, right)
  if (operation === '*') return multiplyRationals(left, right)
  return divideRationals(left, right)
}

function formatNumber(value: number): string {
  if (Math.abs(value) < EPSILON) return '0'
  if (Number.isInteger(value)) return String(value)
  return Number(value.toFixed(8)).toString()
}

function formatPolynomial(poly: Polynomial): string {
  const cleanPoly = trimPolynomial(poly)
  if (isZeroPolynomial(cleanPoly)) return '0'

  const terms: string[] = []

  for (let degree = cleanPoly.length - 1; degree >= 0; degree -= 1) {
    const coefficient = cleanPoly[degree]
    if (Math.abs(coefficient) < EPSILON) continue

    const absoluteCoefficient = Math.abs(coefficient)
    const sign = coefficient < 0 ? '-' : '+'
    const variablePart = degree === 0 ? '' : degree === 1 ? 'x' : `x^${degree}`
    const coefficientPart =
      degree > 0 && Math.abs(absoluteCoefficient - 1) < EPSILON
        ? ''
        : formatNumber(absoluteCoefficient)
    const term = `${coefficientPart}${variablePart}`

    if (terms.length === 0) {
      terms.push(coefficient < 0 ? `-${term}` : term)
    } else {
      terms.push(`${sign} ${term}`)
    }
  }

  return terms.join(' ')
}

function formatCoefficientList(poly: Polynomial): string {
  return trimPolynomial(poly).map(formatNumber).join(', ')
}

function formatRational(rational: RationalPolynomial): string {
  const numerator = trimPolynomial(rational.numerator)
  const denominator = trimPolynomial(rational.denominator)

  if (isZeroPolynomial(numerator)) {
    return '0'
  }

  if (denominator.length === 1 && Math.abs(denominator[0] - 1) < EPSILON) {
    return formatPolynomial(numerator)
  }

  return `(${formatPolynomial(numerator)}) / (${formatPolynomial(denominator)})`
}

export default function App() {
  const [pInput, setPInput] = useState('1, 2, 3')
  const [qInput, setQInput] = useState('4, -1')
  const [xInput, setXInput] = useState('2')
  const [result, setResult] = useState('Select an operation.')
  const [detail, setDetail] = useState('C++ WebAssembly engine')
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

  const parsedPreview = useMemo(() => {
    try {
      const p = parsePolynomial(pInput)
      const q = parsePolynomial(qInput)
      return {
        p: formatPolynomial(p),
        q: formatPolynomial(q),
        pCoefficients: formatCoefficientList(p),
        qCoefficients: formatCoefficientList(q),
      }
    } catch {
      return null
    }
  }, [pInput, qInput])

  function handleEngineResult(engineResult: string) {
    setResult(engineResult)
    if (engineResult.startsWith('Error:')) {
      setError(engineResult)
    } else {
      setError('')
    }
  }

  function runFallbackOperation(operation: BinaryOperation): string {
    const p = parsePolynomial(pInput)
    const q = parsePolynomial(qInput)
    const rational = applyOperation(operation, p, q)
    return formatRational(rational)
  }

  function runOperation(operation: BinaryOperation) {
    try {
      if (wasmEngine) {
        const operationMap = {
          '+': wasmEngine.add,
          '-': wasmEngine.subtract,
          '*': wasmEngine.multiply,
          '/': wasmEngine.divide,
        }
        handleEngineResult(operationMap[operation](pInput, qInput))
        setDetail(`WASM: P ${operation} Q`)
        return
      }

      handleEngineResult(runFallbackOperation(operation))
      setDetail(`Fallback oracle: P ${operation} Q`)
    } catch (operationError) {
      setError(operationError instanceof Error ? operationError.message : String(operationError))
    }
  }

  function runRemainder() {
    try {
      if (wasmEngine) {
        handleEngineResult(wasmEngine.remainder(pInput, qInput))
        setDetail('WASM: remainder after P / Q')
        return
      }

      const { quotient, remainder } = dividePolynomials(parsePolynomial(pInput), parsePolynomial(qInput))
      handleEngineResult(formatPolynomial(remainder))
      setDetail(`Fallback oracle: remainder after P / Q, quotient ${formatPolynomial(quotient)}`)
    } catch (operationError) {
      setError(operationError instanceof Error ? operationError.message : String(operationError))
    }
  }

  function runDerivative() {
    try {
      if (wasmEngine) {
        handleEngineResult(wasmEngine.derivative(pInput))
        setDetail('WASM: derivative of P')
        return
      }

      handleEngineResult(formatPolynomial(derivative(parsePolynomial(pInput))))
      setDetail('Fallback oracle: derivative of P')
    } catch (operationError) {
      setError(operationError instanceof Error ? operationError.message : String(operationError))
    }
  }

  function runEvaluation() {
    try {
      const xValue = Number(xInput)
      if (!Number.isFinite(xValue)) {
        throw new Error('Enter a numeric value for a.')
      }

      if (wasmEngine) {
        handleEngineResult(wasmEngine.evaluate(pInput, xValue))
        setDetail(`WASM: P(${formatNumber(xValue)})`)
        return
      }

      handleEngineResult(formatNumber(evaluate(parsePolynomial(pInput), xValue)))
      setDetail(`Fallback oracle: P(${formatNumber(xValue)})`)
    } catch (operationError) {
      setError(operationError instanceof Error ? operationError.message : String(operationError))
    }
  }

  function loadExample(example: (typeof examples)[number]) {
    setPInput(example.p)
    setQInput(example.q)
    setError('')
  }

  return (
    <main className="app-shell">
      <section className="workspace" aria-labelledby="app-title">
        <header className="workspace-header">
          <div>
            <p className="eyebrow">Rational polynomial engine</p>
            <h1 id="app-title">Polynomial Calculator</h1>
          </div>
          <div className={`engine-status ${wasmStatus}`}>
            <span>WASM engine: {wasmStatus}</span>
            {wasmStatus === 'error' ? <small>{wasmError}</small> : null}
          </div>
        </header>

        <div className="calculator-grid">
          <section className="input-panel" aria-label="Polynomial inputs">
            <label className="field">
              <span>P(x)</span>
              <textarea
                value={pInput}
                onChange={(event) => setPInput(event.target.value)}
                spellCheck={false}
                rows={4}
              />
            </label>

            <label className="field">
              <span>Q(x)</span>
              <textarea
                value={qInput}
                onChange={(event) => setQInput(event.target.value)}
                spellCheck={false}
                rows={4}
              />
            </label>

            <div className="examples" aria-label="Examples">
              {examples.map((example) => (
                <button key={example.label} type="button" onClick={() => loadExample(example)}>
                  {example.label}
                </button>
              ))}
            </div>
          </section>

          <section className="operation-panel" aria-label="Operations">
            <div className="operation-grid">
              <button type="button" onClick={() => runOperation('+')}>
                P + Q
              </button>
              <button type="button" onClick={() => runOperation('-')}>
                P - Q
              </button>
              <button type="button" onClick={() => runOperation('*')}>
                P * Q
              </button>
              <button type="button" onClick={() => runOperation('/')}>
                P / Q
              </button>
              <button type="button" onClick={runRemainder}>
                Remainder
              </button>
              <button type="button" onClick={runDerivative}>
                dP/dx
              </button>
            </div>

            <label className="field compact">
              <span>Evaluate P at x = a</span>
              <div className="inline-control">
                <input value={xInput} onChange={(event) => setXInput(event.target.value)} />
                <button type="button" onClick={runEvaluation}>
                  Evaluate
                </button>
              </div>
            </label>
          </section>

          <section className="result-panel" aria-live="polite" aria-label="Result">
            <div className="result-heading">
              <span>{detail}</span>
            </div>
            <output>{result}</output>
            {error ? <p className="error">{error}</p> : null}
          </section>
        </div>

        <section className="preview-band" aria-label="Parsed polynomials">
          <div>
            <span>P(x)</span>
            <strong>{parsedPreview?.p ?? 'Invalid coefficients'}</strong>
            <small>{parsedPreview ? `[${parsedPreview.pCoefficients}]` : ''}</small>
          </div>
          <div>
            <span>Q(x)</span>
            <strong>{parsedPreview?.q ?? 'Invalid coefficients'}</strong>
            <small>{parsedPreview ? `[${parsedPreview.qCoefficients}]` : ''}</small>
          </div>
        </section>
      </section>
    </main>
  )
}

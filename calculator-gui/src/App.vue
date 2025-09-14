<template>
  <div id="app">
    <header>
      <h1>Vue Calculator App</h1>
      <button @click="toggleMode">
        {{ calculatorMode ? 'Switch to Complex/Polynomial Mode' : 'Switch to Basic Calculator Mode' }}
      </button>
    </header>

    <div v-if="calculatorMode">
      <BasicCalculator />
    </div>
    <div v-else>
      <ComplexPolynomialCalculator />
    </div>
  </div>
</template>

<script>
import BasicCalculator from './components/BasicCalculator.vue';
import ComplexPolynomialCalculator from './components/ComplexPolynomialCalculator.vue';

export default {
  name: 'App',
  components: {
    BasicCalculator,
    ComplexPolynomialCalculator,
  },
  data() {
    return {
      calculatorMode: true, // true for basic, false for complex/polynomial
    };
  },
  methods: {
    toggleMode() {
      this.calculatorMode = !this.calculatorMode;
    },
  },
};
</script>

<style>
#app {
  text-align: center;
  margin-top: 50px;
}

header {
  margin-bottom: 20px;
}
</style>

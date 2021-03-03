module.exports = {
  root: true,
  env: {
    node: true,
  },
  extends: [
    'airbnb-typescript/base',
    'plugin:unicorn/recommended',
    'plugin:vue/recommended',

    'prettier',
    'prettier/@typescript-eslint',
    'prettier/unicorn',
    'prettier/vue',
  ],
  globals: {
    'AMap': 'readonly',
    'Loca': 'readonly',
  },
  parser: 'vue-eslint-parser',
  parserOptions: {
    parser: '@typescript-eslint/parser',
    ecmaVersion: 2020,
    project: './tsconfig.json',
    extraFileExtensions: ['vue'],
  },
  settings: {
    'import/resolver': {
      webpack: {
        config: require.resolve('@vue/cli-service/webpack.config.js'),
      },
    },
  },
  rules: {
    'no-console': process.env.NODE_ENV === 'production' ? 'warn' : 'off',
    'no-debugger': process.env.NODE_ENV === 'production' ? 'warn' : 'off',
    quotes: ['error', 'single'],
    semi: ['error', 'always'],
    'unicorn/prefer-optional-catch-binding': 0,
    'no-underscore-dangle': 0,
    'class-methods-use-this': 0,
    'unicorn/no-null': 0,
    'no-bitwise': 0,
    'unicorn/no-reduce': 0,
    'no-lone-blocks': 0,
    'unicorn/no-abusive-eslint-disable': 0,
    'no-plusplus': 0,
    'no-multi-assign': 0,
    'import/no-cycle': 0,
    'unicorn/prefer-set-has': 0,
    'unicorn/filename-case': [
      'error',
      {
        case: 'kebabCase',
        ignore: ['^[A-Za-z]+\\.vue$'],
      },
    ],
    '@typescript-eslint/lines-between-class-members': [
      'error',
      {
        exceptAfterSingleLine: true,
      },
    ],
    // add
    'no-param-reassign': 0,
  },
  overrides: [{
    files: [
      '**/__tests__/*.{j,t}s?(x)',
      '**/tests/unit/**/*.spec.{j,t}s?(x)',
    ],
    env: {
      jest: true,
    },
  }, ],
};
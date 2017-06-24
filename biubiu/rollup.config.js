import pkg from './package.json';

export default {
	entry: 'src/main.js',
	targets: [{
		dest: pkg.main,
		format: 'iife',
		moduleName: 'Biubiu',
		sourceMap: true,
	}],
};

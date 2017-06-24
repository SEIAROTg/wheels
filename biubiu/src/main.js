/*
options:
	data
	props
	computed
	methods
	watch
	el
	template
	render

	beforeCreate
	created
	beforeMount
	mounted
	beforeUpdate
	updated
	activated
	deactivated
	beforeDestroy
	destroyed

	parent
	mixins
	extends

	// delimiters    ["{{", "}}"]
*/

import compile from './compiler';
import render from './render';
import watch from './watch';


export default class Biubiu {
	constructor(options = {}) {
		const $options = Object.assign({}, this.constructor.options, options);
		Object.defineProperty(this, '$options', { value: $options });
		const compiled = this.$options.render || Biubiu.compile(this.$options.template);
		const data = options.data instanceof Function ? options.data() : options.data;
		const [el, bindEls] = render(compiled, this);
		watch(this, data, $options.computed, compiled.computeds, bindEls);
		Object.defineProperty(this, '$el', { value: el });
		if ($options.methods) {
			for (const [name, func] of Object.entries($options.methods)) {
				if (name.startsWith('$')) {
					throw new Error('method name cannot start with "$"');
				}
				Object.defineProperty(this, name, { value: func });
			}
		}
		this.$mount(options.el);
	}

	static compile(selectorOrHtml) {
		return compile(selectorOrHtml);
	}

	static extend(extendedOptions) {
		if (extendedOptions.data && !extendedOptions.data instanceof Function) {
			throw new Error('options.data must be function');
		}
		const parent = this;
		const child = function(options) {
			return new parent(Object.assign({}, extendedOptions, options));
		};
		child.prototype = parent.prototype;
		child.__proto__ = parent;
		extendedOptions = Object.assign({}, extendedOptions);
		if (extendedOptions.template && !extendedOptions.render) {
			extendedOptions.render = Biubiu.compile(extendedOptions.template);
		}
		return child;
	}

	static nextTick() {

	}

	static set() {

	}

	static delete() {

	}

	static directive() {

	}

	static filter() {

	}

	static component() {

	}

	static use() {

	}

	static mixin() {

	}

	static version() {
		return 0;
	}

	// $data() {
	// $props() {
	// $el() {
	// $options() {
	// $parent() {
	// $root() {
	// $children() {
	// $slots() {
	// $refs() {

	$watch() {

	}

	$set() {

	}

	$delete() {

	}

	$on() {

	}

	$once() {

	}

	$off() {

	}

	$emit() {

	}

	$mount(elementOrSelector) {
		if (typeof elementOrSelector === 'string') {
			elementOrSelector = document.querySelector(elementOrSelector);
		}
		elementOrSelector.parentNode.replaceChild(this.$el, elementOrSelector);
		this.$options.mounted && this.$options.mounted.call(this);
	}

	$forceUpdate() {

	}

	$nextTick() {

	}

	$destroy() {

	}
};

Biubiu.options = {};

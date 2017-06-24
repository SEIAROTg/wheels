function createElement(tagName, attrs, children) {
	const el = document.createElement(tagName);
	for (const [key, value] of Object.entries(attrs)) {
		el.setAttributes(key, value);
	}
	for (const child of children) {
		el.appendChild(child);
	}
	return el;
}

function createTextNode(text) {
	return document.createTextNode(text);
}

export default function render(compiled, vm) {
	const bindEls = [];

	function createComputedTextNode(computeds, computedId) {
		const el = document.createTextNode('');
		bindEls[computedId] = el;
		return el;
	}

	const el = compiled.render.call(
		vm,
		createElement,
		createTextNode,
		createComputedTextNode.bind(vm, compiled.computeds)
	);

	return [el, bindEls];
};

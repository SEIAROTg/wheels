import { escapeString } from './utils';

export default function compile(selectorOrHtml) {
	const templateStr = selectorOrHtml.startsWith('#')
		? document.querySelector(selectorOrHtml).innerHTML
		: selectorOrHtml;

	const templateEl = document.createElement('template');
	templateEl.innerHTML = templateStr;
	const fragment = templateEl.content;

	if (fragment.children.length !== 1
		|| [].filter.call(fragment.childNodes, n => n.nodeType === Node.TEXT_NODE && n.textContent.trim()).length
	) {
		throw new Error('template must contain exactly one element');
	}

	const el = fragment.firstElementChild;
	const computeds = [];

	function generateCreateElementCode(node) {
		switch (node.nodeType) {
		case Node.ELEMENT_NODE:
			const children = [].concat.apply([], [].map.call(node.childNodes, generateCreateElementCode));
			const attrs = {};
			for (const attr of node.attributes) {
				attrs[attr.name] = attr.value;
			}
			return [`e('${node.tagName}',${JSON.stringify(attrs)},[${children.join(',')}])`];
			break;
		case Node.TEXT_NODE:
			const segs = node.textContent.split(/\{\{(.*?)\}\}/);
			const rets = [];
			if (segs[0]) {
				rets.push(`t('${escapeString(segs[0])}')`);
			}
			for (let i = 1; i < segs.length; i += 2) {
				const computed = new Function([], `with(this){return ${segs[i]};}`);
				computeds.push(computed);
				rets.push(`c(${computeds.length - 1})`);
				if (segs[i + 1]) {
					rets.push(`t('${escapeString(segs[i + 1])}')`);
				}
			}
			return rets;
			break;
		default:
			return [];
		}
	}
	const createElementCode = generateCreateElementCode(el);
	return {
		render: new Function(['e', 't', 'c'], `return ${createElementCode};`),
		computeds,
	}
};

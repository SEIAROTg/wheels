export function escapeString(str) {
	return str
		.replace(/\\/g, '\\\\')
		.replace(/'/g, '\\\'')
		.replace(/"/g, '\\\"')
		.replace(/\n/g, '\\\n');
};

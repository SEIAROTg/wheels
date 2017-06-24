export default function watch(vm, data, computeds = {}, anonymousComputeds, bindEls) {
	let notifier = null;
	let notifierClearers = null;

	for (const [key, value] of Object.entries(data)) {
		if (key.startsWith('$')) {
			throw new Error('name of data property cannot start with "$"');
		}
		
		!function(value) {
			const notifiers = new Set();
			Object.defineProperty(vm, key, {
				get() {
					notifier && notifiers.add(notifier);
					notifierClearers && notifierClearers.add(notifierCleaner);
					return value;
				},
				set(newValue) {
					value = newValue;
					for (const notifier of [...notifiers]) {
						notifier();
					}
				},
			});
			function notifierCleaner(notifier) {
				notifiers.delete(notifier);
			}
		}(value);
	}
	
	for (const [key, func] of Object.entries(computeds)) {
		if (key.startsWith('$')) {
			throw new Error('name of computed property cannot start with "$"');
		}
		!function() {
			let ready = false;
			const notifiers = new Set();
			const _notifierClearers = new Set();
			let value;

			Object.defineProperty(vm, key, {
				get() {
					if (!ready) {
						update();
					}
					notifier && notifiers.add(update);
					notifierClearers && notifierClearers.add(notifierCleaner);
					return value;
				},
			});

			function update() {
				for (const _notifierClearer of _notifierClearers) {
					_notifierClearer(update);
				}
				_notifierClearers.clear();
				const oldNotifier = notifier;
				const oldNotifierCleaners = notifierClearers;
				notifier = update;
				notifierClearers = _notifierClearers;
				let value;
				try {
					value = func.call(vm);
				} catch (e) {
					console.error(e);
					value = '';
				}
				notifier = oldNotifier;
				notifierClearers = oldNotifierCleaners;
				ready = true;
				for (const notifier of [...notifiers]) {
					notifier();
				}
			}

			function notifierCleaner(notifier) {
				notifiers.delete(notifier);
			}
		}();
	}

	for (const [id, func] of anonymousComputeds.entries()) {
		!function() {
			const _notifierClearers = new Set();
			let value;

			function update() {
				for (const _notifierClearer of _notifierClearers) {
					_notifierClearer(update);
				}
				_notifierClearers.clear();
				const oldNotifier = notifier;
				const oldNotifierCleaners = notifierClearers;
				notifier = update;
				notifierClearers = _notifierClearers;
				let value;
				try {
					value = func.call(vm);
				} catch (e) {
					console.error(e);
					value = '';
				}
				notifier = oldNotifier;
				notifierClearers = oldNotifierCleaners;
				bindEls[id].textContent = value;
			}

			update();
		}();
	}

};

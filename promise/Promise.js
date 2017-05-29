'use strict';

const util = require('util');

function thenable(object) {
	return object && object.then;
}

class MyPromise {
	constructor(callback) {
		this.state = 'pending';
		this.thens = [];
		this.catches = [];
		try {
			callback((res) => {
				if (this.state !== 'pending') return;
				this.state = 'resolved';
				this.res = res;
				this.thens.filter(x => x(res) && false);
			}, (err) => {
				if (this.state !== 'pending') return;
				this.state = 'rejected';
				this.err = err;
				this.catches.filter(x => x(err) && false);
			});
		} catch (err) {
			this.state = 'rejected';
			this.err = err;
			this.catches.filter(x => x(err) && false);
		}
	}

	then(resolveCallback, rejectCallback) {
		switch (this.state) {
		case 'pending':
			let nextResolve, nextReject;
			if (resolveCallback) {
				this.thens.push((res) => {
					try {
						const nextPromise = resolveCallback(res);
						if (thenable(nextPromise)) {
							nextPromise.then(nextResolve, nextReject);
						} else {
							nextResolve(nextPromise);
						}
					} catch (err) {
						nextReject(err);
						return;
					}
				});
			}
			if (rejectCallback) {
				this.catches.push((err) => {
					try {
						const nextPromise = rejectCallback(err);
						if (thenable(nextPromise)) {
							nextPromise.then(nextResolve, nextReject);
						} else {
							nextResolve(nextPromise);
						}
					} catch (err) {
						nextReject(err);
						return;
					}
				});
			} else {
				this.catches.push((err) => {
					nextReject(err);
				});
			}
			return new MyPromise((resolve, reject) => {
				nextResolve = resolve;
				nextReject = reject;
			});
		case 'resolved':
			return resolveCallback(this.res);
		case 'rejected':
			return rejectCallback(this.err);
		}
	}

	catch(callback) {
		return this.then((res) => MyPromise.resolve(res), callback);
	}

	static resolve(res) {
		return new MyPromise((resolve) => resolve(res));
	}

	static reject(err) {
		return new MyPromise((resolve, rejected) => rejected(err));
	}

	[util.inspect.custom]() {
		switch (this.state) {
		case 'pending':
			return 'Promise { <pending> }';
		case 'resolved':
			return `Promise { ${this.res} }`;
		case 'rejected':
			return `Promise { <rejected> ${this.err} }`;
		}
	}
};

module.exports = MyPromise;

function sleep(time) {
	return new MyPromise((resolve, reject) => {
		setTimeout(() => {
			resolve();
		}, time);
	});
}

sleep(1000)
.then(() => {
	console.log('sleep 1');
	return sleep(1000);
}, () => {
	console.log('catch 1');
})
.then(() => {
	console.log('sleep 2');
	return sleep(1000);
}, () => {
	console.log('catch 2');
})
.then(() => {
	console.log('sleep 3');
	throw 1;
	return sleep(1000);
}, () => {
	console.log('catch 3');
})
.then(() => {
	console.log('sleep 4');
	return sleep(1000);
})
.then(() => {
	console.log('sleep 5');
}, () => {
	console.log('catch 5');
})
.then(() => {
	console.log('sleep 6');
}, () => {
	console.log('catch 6');
});

let canvas = document.getElementById('canvas');
let ctx = canvas.getContext('2d');
let SIZE = 1;
let LIMIT = 1e10;
let ITERS = 1000;
let COLOR_SMOOTH = 0.2;

// Color smoothing: https://en.wikipedia.org/wiki/Mandelbrot_set
function hslToRgb(n,r,t){var u,o,a;if(0==r)u=o=a=t;else{var e=function(n,r,t){return t<0&&(t+=1),t>1&&(t-=1),t<1/6?n+6*(r-n)*t:t<.5?r:t<2/3?n+(r-n)*(2/3-t)*6:n},h=t<.5?t*(1+r):t+r-t*r,d=2*t-h;u=e(d,h,n+1/3),o=e(d,h,n),a=e(d,h,n-1/3)}return[Math.round(255*u),Math.round(255*o),Math.round(255*a)]}

//let c = math.complex(0.5, -0.2);
let c = math.complex(-0.8, 0.156);
function foo(z) {
	return math.add(math.square(z), c);
}

function iterate(z, f, iters) {
	for (let i = 0; i < iters; i++) {
		z = foo(z);
		if (!converges(z)) {
			let n = i;
			let v = 1+n - math.log(math.log(z.abs())/math.log(LIMIT))/math.log(2);
			return v / iters;
			//return math.pow(v * 1e-2, 5);
		}
	}
	//console.log(z);
	return -1.0;
}

function converges(z) {
	return Math.abs(z.re) < LIMIT || Math.abs(z.im) < LIMIT;
}

function main() {
	let iData = ctx.getImageData(0, 0, canvas.width, canvas.height);
	for (let i = 0; i < iData.data.length; i += 4) {

		let x = ((i/4)%canvas.width) * (SIZE*2/canvas.width) - SIZE;
		let y = SIZE - (parseInt((i/4)/canvas.width) * (SIZE*2/canvas.height));

		let color = iterate(math.complex(x, y), foo, ITERS);
		let rgb;
		if (color < 0.0)
			rgb = [0, 0, 0];
		else
			rgb = hslToRgb(math.pow(color, COLOR_SMOOTH), 1, 0.5);
		iData.data[i] = rgb[0];
		iData.data[i+1] = rgb[1];
		iData.data[i+2] = rgb[2];
		iData.data[i+3] = 0xff;
	}
	ctx.putImageData(iData, 0, 0);
}

//let x = iterate(math.complex(2, 2), foo, 50);
//console.log(x);
main();

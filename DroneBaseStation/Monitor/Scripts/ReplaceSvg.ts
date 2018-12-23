document.addEventListener("DOMContentLoaded", ReplaceSvg);

function ReplaceSvg(): void {
	document.querySelectorAll('img.svg').forEach(function (img: any) {
		let imgID: string = img.id;
		let imgClass: string = img.className;
		let imgURL: string = img.src;

		fetch(imgURL).then(function (response: Response) {
			return response.text();
		}).then(function (text: string) {

			let parser: DOMParser = new DOMParser();
			let xmlDoc: Document = parser.parseFromString(text, "text/xml");

			// Get the SVG tag, ignore the rest
			let svg: SVGSVGElement = xmlDoc.getElementsByTagName('svg')[0];

			// Add replaced image's ID to the new SVG
			if (typeof imgID !== 'undefined') {
				svg.setAttribute('id', imgID);
			}
			// Add replaced image's classes to the new SVG
			if (typeof imgClass !== 'undefined') {
				svg.setAttribute('class', imgClass + ' replaced-svg');
			}

			// Remove any invalid XML tags as per http://validator.w3.org
			svg.removeAttribute('xmlns:a');

			// Check if the viewport is set, if the viewport is not set the SVG wont't scale.
			if (!svg.getAttribute('viewBox') && svg.getAttribute('height') && svg.getAttribute('width')) {
				svg.setAttribute('viewBox', '0 0 ' + svg.getAttribute('height') + ' ' + svg.getAttribute('width'))
			}

			// Replace image with new SVG
			img.parentNode.replaceChild(svg, img);
		});
	});
}
/*
*       |\____/| github.com/CiupagaPL/Przecinek
*       |      | original {,} wiki
*  __   | >  < |
* /  \__\ =, = / copyright 2026
* \__         /
*    \ \____\ \
*    {,{,} {,},}
*/

// Setup global [localStorage]
if(localStorage.getItem("animation")===null){
	// Create [localStorage] `animation`
	localStorage.setItem("animation", "true");
}
if(localStorage.getItem("light")===null){
	// Create [localStorage] `light`
	localStorage.setItem("light", "true");
}
if(localStorage.getItem("language")===null){
	// Create [localStorage] `language`
	localStorage.setItem("language", "pl");
}
if(localStorage.getItem("scale")===null){
	// Create [localStorage] `scale`
	localStorage.setItem("scale", "0");
}

// Create and load [animation], [light], [language] and [scale] values
let animation=localStorage.getItem("animation"),
	light=localStorage.getItem("light"),
	language=localStorage.getItem("language"),
	scale=localStorage.getItem("scale");

// Create and load [icon] from HTML
const icon=document.getElementById("icon");

// Create and load [navBack] from HTML
const navBack=document.getElementById("navBack");

// Create and load [navFront] from HTML
const navFront=document.getElementById("navFront");

// Create and load [logo] from HTML
const logo=document.getElementById("logo");

// Create, load and setup [sub] from HTML
const sub=document.getElementById("sub");
sub.dataset.key="random"+Math.floor(Math.random()*30);

// Create and load [animationsSetting], [themeSetting] and [scaleSetting]
const animationsSetting=document.getElementById("animationsSetting");
const themeSetting=document.getElementById("themeSetting");
const scaleSetting=document.getElementById("scaleSetting");

// Create and load [main] from HTML
const main=document.getElementById("main");

// Create and load [translation] from JSON
const translation=JSON.parse(document.getElementById("data").textContent);

if(animation==="true"){
	// Setup [logo] animation
	logo.classList.add("blue-red-on");
}
else{
	// Disable [navBack] animation
	navBack.classList.add("off");

	// Disable [logo] animation
	logo.classList.add("blue-red-off");
}

if(light==="false"){
	// Set [navBack] and [navFront] colorscheme
	navBack.classList.add("dark");
	navFront.classList.add("dark");

	// Set [main] colorscheme
	main.classList.add("dark");
}

// Set [main] scale
if(scale==="1"){ main.classList.add("scale2"); }
else if(scale==="2"){ main.classList.add("scale3"); }

/*
* |\____/| refreshButtons()
* |      |
* | o  o | [JS]
* \ = .= /
*/
function refreshButtons(){
	// Create and calculate local [position] for [animationsSetting]
	let position=animationsSetting.innerHTML.indexOf(" ");
	if(position!==(-1)){
		// Cut [animationsSetting] text
		animationsSetting.innerHTML=
			animationsSetting.innerHTML.slice(0, position+1);
	}

	// Update [document] [animations] based on [animation] value
	if(animation==="true"){ animationsSetting.innerHTML+="✓"; }
	else{ animationsSetting.innerHTML+="⨯"; }

	// Calculate local [position] for [themeSetting]
	position=themeSetting.innerHTML.indexOf(" ");
	if(position!==(-1)){
		// Cut [themeSetting] text
		themeSetting.innerHTML=
			themeSetting.innerHTML.slice(0, position+1);
	}

	// Update [document] [theme] based on [light] value
	if(light==="true"){ themeSetting.innerHTML+="☼"; }
	else{ themeSetting.innerHTML+="☽"; }

	// Calculate local [position] for [scaleSetting]
	position=scaleSetting.innerHTML.indexOf(" ");
	if(position!==(-1)){
		// Cut [scaleSetting] text
		scaleSetting.innerHTML=
			scaleSetting.innerHTML.slice(0, position+1);
	}

	// Update [document] [theme] based on [light] value
	if(scale==="0"){ scaleSetting.innerHTML+="1x"; }
	else if(scale==="1"){ scaleSetting.innerHTML+="1.5x"; }
	else{ scaleSetting.innerHTML+="2x"; }
}

/*
* |\____/| refreshSub()
* |      |
* | o  o | [JS]
* \ = .= /
*/
function refreshSub(){
	// Create and calculate local [left], [top], [size] and [adjust]
	const left=11.5;
	const top=(sub.innerHTML.length*42)/21;
	const size=(21*1.2)/sub.innerHTML.length;
	const adjust=sub.innerHTML.length/5;

	// Update [sub] position and size
	sub.style.marginLeft=left+"vw";
	sub.style.marginTop=top+size-(8.5*(adjust-1))+"vw";
	sub.style.fontSize=size+"vw";
}

/*
* |\____/| changeLanguage()
* |      |
* | o  o | [JS]
* \ = .= /
*/
function changeLanguage(){
    // Scan each `data-key`
    document.querySelectorAll("[data-key]").forEach(current=>{
		// Create and load local [key]
        const key=current.getAttribute("data-key");

		// Create and load local [text]
        let text=translation[language][key];

        if(text!==null){
			// Apply [text] for [current] element
			current.textContent=text;
        }
    });

	// Refresh buttons content
	refreshButtons();

	// Refresh [sub] position and size
	refreshSub();
}

// Update site content on start
changeLanguage();

/*
* |\____/| animationsPress()
* |      |
* | o  o | [JS]
* \ = .= /
*/
function animationsPress(){
	if(animation==="true"){
		// Update [animation] value
		animation="false";

		// Disable [navBack] animation
		navBack.classList.add("off");

		// Disable [logo] animation
		if(logo.classList.contains("red-green-on")===true){
			logo.classList.remove("red-green-on");
			logo.classList.add("red-green-off");
		}
		else if(logo.classList.contains("green-blue-on")===true){
			logo.classList.remove("green-blue-on");
			logo.classList.add("green-blue-off");
		}
		else{
			logo.classList.remove("blue-red-on");
			logo.classList.add("blue-red-off");
		}
	}
	else{
		// Update [animation] value
		animation="true";

		// Enable [navBack] animation
		navBack.classList.remove("off");

		// Enable [logo] animation
		if(logo.classList.contains("red-green-off")===true){
			logo.classList.remove("red-green-off");
			logo.classList.add("red-green-on");
		}
		else if(logo.classList.contains("green-blue-off")===true){
			logo.classList.remove("green-blue-off");
			logo.classList.add("green-blue-on");
		}
		else{
			logo.classList.remove("blue-red-off");
			logo.classList.add("blue-red-on");
		}
	}

	// Update [localStorage]
	localStorage.setItem("animation", animation);

	// Refresh buttons content
	refreshButtons();
}

/*
* |\____/| themePress()
* |      |
* | o  o | [JS]
* \ = .= /
*/
function themePress(){
	if(light==="true"){
		// Update [light] value
		light="false";

		// Change [navBack] and [navFront] colorscheme
		navBack.classList.add("dark");
		navFront.classList.add("dark");

		// Change [main] colorscheme
		main.classList.add("dark");
	}
	else{
		// Update [light] value
		light="true";

		// Change [navBack] and [navFront] colorscheme
		navBack.classList.remove("dark");
		navFront.classList.remove("dark");

		// Change [main] colorscheme
		main.classList.remove("dark");
	}

	// Update [localStorage]
	localStorage.setItem("light", light);

	// Refresh buttons content
	refreshButtons();
}

/*
* |\____/| languagePress()
* |      |
* | o  o | [JS]
* \ = .= /
*/
function languagePress(){
	// Update [language] value
	if(language==="en"){ language="pl"; }
	else{ language="en"; }

	// Update [localStorage]
	localStorage.setItem("language", language);

	// Refresh site content
	changeLanguage();
}

/*
* |\____/| scalePress()
* |      |
* | o  o | [JS]
* \ = .= /
*/
function scalePress(){
	if(scale==="0"){
		// Update [scale] value
		scale="1";

		// Update [main] scale
		main.classList.add("scale2");
	}
	else if(scale==="1"){
		// Update [scale] value
		scale="2";

		// Update [main] scale
		main.classList.remove("scale2");
		main.classList.add("scale3");
	}
	else{
		// Update [scale] value
		scale="0";

		// Update [main] scale
		main.classList.remove("scale3");
	}

	// Update [localStorage]
	localStorage.setItem("scale", scale);

	// Refresh buttons content
	refreshButtons();
}

/*
* |\____/| logo.addEventListener("mouseenter")
* |      |
* | o  o | [JS]
* \ = .= /
*/
logo.addEventListener("mouseenter", ()=>{
	if(logo.classList.contains("red-green-on")===true){
		// Change [logo] animation
		logo.classList.remove("red-green-on");
		logo.classList.add("green-blue-on");

		// Change [icon] source
		icon.href="local/icon_blue.png"+"?v="+Date.now();
	}
	else if(logo.classList.contains("green-blue-on")===true){
		// Change [logo] animation
		logo.classList.remove("green-blue-on");
		logo.classList.add("blue-red-on");

		// Change [icon] source
		icon.href="local/icon_red.png"+"?v="+Date.now();
	}
	else if(logo.classList.contains("blue-red-on")===true){
		// Change [logo] animation
		logo.classList.remove("blue-red-on");
		logo.classList.add("red-green-on");

		// Change [icon] source
		icon.href="local/icon_green.png"+"?v="+Date.now();
	}

	else if(logo.classList.contains("red-green-off")===true){
		// Change [logo] color
		logo.classList.remove("red-green-off");
		logo.classList.add("green-blue-off");

		// Change [icon] source
		icon.href="local/icon_blue.png"+"?v="+Date.now();
	}
	else if(logo.classList.contains("green-blue-off")===true){
		// Change [logo] color
		logo.classList.remove("green-blue-off");
		logo.classList.add("blue-red-off");

		// Change [icon] source
		icon.href="local/icon_red.png"+"?v="+Date.now();
	}
	else{
		// Change [logo] color
		logo.classList.remove("blue-red-off");
		logo.classList.add("red-green-off");

		// Change [icon] source
		icon.href="local/icon_green.png"+"?v="+Date.now();
	}
});
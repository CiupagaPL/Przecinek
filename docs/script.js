/****************************************************
*       |\____/|
*       |      | github.com/CiupagaPL/Przecinek
*  __   | >  < | original {,} wiki
* /  \__\ =, = / copyright 2026
* \__         /
*    \ \____\ \
*    {,{,} {,},}
*****************************************************/

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

// Create and load [animation], [light] and [language]
let animation=localStorage.getItem("animation"),
	light=localStorage.getItem("light"),
	language=localStorage.getItem("language");

// Create and load [icon] from HTML
const icon=document.getElementById("icon");

// Create and load [body] from HTML
const body=document.getElementById("body");

// Create and load [navA] from HTML
const navA=document.getElementById("navA");

// Create and load [logo] from HTML
const logo=document.getElementById("logo");

// Create and calculate [random]
const random=Math.floor(Math.random()*22);

// Create, load and setup [sub] from HTML
const sub=document.getElementById("sub");
sub.dataset.key="random"+random;

// Create and load [navB] from HTML
const navB=document.getElementById("navB");

// Create and load local [animationsSetting], [themeSetting] and [languageSetting]
const animationsSetting=document.getElementById("animationsSetting");
const themeSetting=document.getElementById("themeSetting");
const languageSetting=document.getElementById("languageSetting");

// Create and load [main] from HTML
const main=document.getElementById("main");

// Create and load [translation] from JSON
const translation=JSON.parse(document.getElementById("data").textContent);

if(animation==="true"){
	// Setup [body] animation
	body.classList.add("on");

	// Setup [logo] animation
	logo.classList.add("blue-red-on");
}
else{
	// Disable [body] animation
	body.classList.add("off");

	// Disable [logo] animation
	logo.classList.add("blue-red-off");
}

if(light==="true"){
	// Set [body] colorscheme
	body.classList.add("light");

	// Set [navA] and [navB] colorscheme
	navA.classList.add("light");
	navB.classList.add("light");

	// Set [sub] colorscheme
	sub.classList.add("light");

	// Set [animationsSetting] colorscheme
	animationsSetting.classList.add("light");
	// Set [themeSetting] colorscheme
	themeSetting.classList.add("light");
	// Set [languageSetting] colorscheme
	languageSetting.classList.add("light");

	// Set [main] colorscheme
	main.classList.add("light");
}
else{
	// Set [body] colorscheme
	body.classList.add("dark");

	// Set [navA] and [navB] colorscheme
	navA.classList.add("dark");
	navB.classList.add("dark");

	// Set [sub] colorscheme
	sub.classList.add("dark");

	// Set [animationsSetting] colorscheme
	animationsSetting.classList.add("dark");
	// Set [themeSetting] colorscheme
	themeSetting.classList.add("dark");
	// Set [languageSetting] colorscheme
	languageSetting.classList.add("dark");

	// Set [main] colorscheme
	main.classList.add("dark");
}

/************************************
* |\____/| refreshButtons()
* |      |
* | o  o | [JS]
* \ = .= /
*************************************/
function refreshButtons(){
	// Create and calculate local [position] for [animationsSetting]
	let position=animationsSetting.innerHTML.indexOf(": ")
	if(position!==(-1)){
		// Cut [animationsSetting] text
		animationsSetting.innerHTML=animationsSetting.innerHTML.slice(0, position+2)
	}

	// Update [document] [animations] based on [animation] value
	if(animation==="true"){ animationsSetting.innerHTML+="✓"; }
	else{ animationsSetting.innerHTML+="⨯"; }

	// Calculate local [position] for [themeSetting]
	position=themeSetting.innerHTML.indexOf(": ")
	if(position!==(-1)){
		// Cut [themeSetting] text
		themeSetting.innerHTML=themeSetting.innerHTML.slice(0, position+2)
	}

	// Update [document] [theme] based on [light] value
	if(light==="true"){ themeSetting.innerHTML+="☼"; }
	else{ themeSetting.innerHTML+="☽"; }
}

/************************************
* |\____/| refreshSub()
* |      |
* | o  o | [JS]
* \ = .= /
*************************************/
function refreshSub(){
	// Create and calculate local [left], [top] and [size]
	const left=10;
	const top=(sub.innerHTML.length*11)/21;
	const size=(21*1.35)/sub.innerHTML.length;

	// Update [sub] position and size
	sub.style.marginLeft=left+"vw";
	sub.style.marginTop=top+(size-1.35)+"vw";
	sub.style.fontSize=size+"vw";
}

/************************************
* |\____/| changeLanguage()
* |      |
* | o  o | [JS]
* \ = .= /
*************************************/
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

	// Refresh buttons values
	refreshButtons();

	// Refresh [sub] position and size
	refreshSub();
}

// Run on start
changeLanguage();

/************************************
* |\____/| animationsPress()
* |      |
* | o  o | [JS]
* \ = .= /
*************************************/
function animationsPress(){
	if(animation==="true"){
		// Update [animation] and [localStorage]
		animation="false";
		localStorage.setItem("animation", animation);

		// Disable [body] animation
		body.classList.remove("on");
		body.classList.add("off");

		if(logo.classList.contains("red-green-on")===true){
			// Disable [logo] animation
			logo.classList.remove("red-green-on");
			logo.classList.add("red-green-off");
		}
		else if(logo.classList.contains("green-blue-on")===true){
			// Disable [logo] animation
			logo.classList.remove("green-blue-on");
			logo.classList.add("green-blue-off");
		}
		else{
			// Disable [logo] animation
			logo.classList.remove("blue-red-on");
			logo.classList.add("blue-red-off");
		}
	}
	else{
		// Update [animation] and [localStorage]
		animation="true";
		localStorage.setItem("animation", animation);

		// Enable [body] animation
		body.classList.remove("off");
		body.classList.add("on");

		if(logo.classList.contains("red-green-off")===true){
			// Enable [logo] animation
			logo.classList.remove("red-green-off");
			logo.classList.add("red-green-on");
		}
		else if(logo.classList.contains("green-blue-off")===true){
			// Enable [logo] animation
			logo.classList.remove("green-blue-off");
			logo.classList.add("green-blue-on");
		}
		else{
			// Enable [logo] animation
			logo.classList.remove("blue-red-off");
			logo.classList.add("blue-red-on");
		}
	}

	// Refresh buttons values
	refreshButtons();
}

/************************************
* |\____/| themePress()
* |      |
* | o  o | [JS]
* \ = .= /
*************************************/
function themePress(){
	if(light==="true"){
		// Update [light] and [localStorage]
		light="false";
		localStorage.setItem("light", light);

		// Change [body] colorscheme
		body.classList.add("dark");
		body.classList.remove("light");

		// Change [navA] and [navB] colorscheme
		navA.classList.add("dark");
		navA.classList.remove("light");
		navB.classList.add("dark");
		navB.classList.remove("light");

		// Change [sub] colorscheme
		sub.classList.add("dark");
		sub.classList.remove("light");

		// Change [animationsSetting] colorscheme
		animationsSetting.classList.add("dark");
		animationsSetting.classList.remove("light");
		// Change [themeSetting] colorscheme
		themeSetting.classList.add("dark");
		themeSetting.classList.remove("light");
		// Change [languageSetting] colorscheme
		languageSetting.classList.add("dark");
		languageSetting.classList.remove("light");

		// Change [main] colorscheme
		main.classList.add("dark");
		main.classList.remove("light");
	}
	else{
		// Update [light] and [localStorage]
		light="true";
		localStorage.setItem("light", light);

		// Change [body] colorscheme
		body.classList.add("light");
		body.classList.remove("dark");

		// Change [navA] and [navB] colorscheme
		navA.classList.add("light");
		navA.classList.remove("dark");
		navB.classList.add("light");
		navB.classList.remove("dark");

		// Change [sub] colorscheme
		sub.classList.add("light");
		sub.classList.remove("dark");

		// Change [animationsSetting] colorscheme
		animationsSetting.classList.add("light");
		animationsSetting.classList.remove("dark");
		// Change [themeSetting] colorscheme
		themeSetting.classList.add("light");
		themeSetting.classList.remove("dark");
		// Change [languageSetting] colorscheme
		languageSetting.classList.add("light");
		languageSetting.classList.remove("dark");

		// Change [main] colorscheme
		main.classList.add("light");
		main.classList.remove("dark");
	}

	// Refresh texts
	changeLanguage();
}

/************************************
* |\____/| languagePress()
* |      |
* | o  o | [JS]
* \ = .= /
*************************************/
function languagePress(){
	if(language==="en"){
		// Update [language] and [localStorage]
		language="pl";
		localStorage.setItem("language", language);

		// Refresh texts
		changeLanguage();
	}
	else{
		// Update [language] and [localStorage]
		language="en";
		localStorage.setItem("language", language);

		// Refresh texts
		changeLanguage();
	}
}

/************************************
* |\____/| logo.addEventListener-
* |      | -("mouseenter")
* | o  o | [JS]
* \ = .= /
*************************************/
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
static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=0.5, user-scalable=yes">
<script>
var connection = new WebSocket("ws://" + location.hostname + ":81/", ["arduino"]);
connection.onopen = function () {
connection.send("Connect " + new Date());
};
connection.onerror = function (error) {
console.log("WebSocket Error ", error);
};
connection.onmessage = function (e) {
  console.log('Server: ', e.data);
  if (e.data[0] == '#') {
    if (e.data[1]=='g'){
    var rgbg = parseInt(e.data.substring(1), 16);
    document.getElementById('rg').value=((rgbg >> 16) & 0xFF);
    document.getElementById('gg').value=((rgbg >> 8) & 0xFF);
    document.getElementById('bg').value=((rgbg >> 0) & 0xFF);     
  }
    if (e.data[1]=='1'){
    var rgb1 = parseInt(e.data.substring(1), 16);
    document.getElementById('r1').value=((rgb1 >> 16) & 0xFF);
    document.getElementById('g1').value=((rgb1 >> 8) & 0xFF);
    document.getElementById('b1').value=((rgb1 >> 0) & 0xFF);
  }
    if (e.data[1]=='2'){
    var rgb2 = parseInt(e.data.substring(1), 16);
    document.getElementById('r2').value=((rgb2 >> 16) & 0xFF);
    document.getElementById('g2').value=((rgb2 >> 8) & 0xFF);
    document.getElementById('b2').value=((rgb2 >> 0) & 0xFF);
  }
   if (e.data[1]=='3'){
    var rgb3 = parseInt(e.data.substring(1), 16);
    document.getElementById('r3').value=((rgb3 >> 16) & 0xFF);
    document.getElementById('g3').value=((rgb3 >> 8) & 0xFF);
    document.getElementById('b3').value=((rgb3 >> 0) & 0xFF);
  }
  }

  if (e.data[0] == '!') {
 if (e.data[1] == 'M') {
    document.getElementById("Hauptknopf").value=e.data.substring(2);}
    if (e.data[1] == 'H') {
        var Hell = parseInt(e.data.substring(2), 10);
        document.getElementById("Bright").value=Hell;
    }
    
    if (e.data[1] == 'F') {
      var FR = parseInt(e.data.substring(2), 10);
     document.getElementById("Hz").value=FR;
    }
     if (e.data[1] == 'B') {
       if (e.data[11] == 'n') {
       document.getElementById("Hz").style.visibility = "visible";
               
          document.getElementById("Zusatzknopfe").style.visibility="visible";
       document.getElementById("Einzelmuster").style.visibility="hidden";
    } 
    if (e.data[11] == 'f') {
    document.getElementById("Hz").style.visibility = "hidden";
  document.getElementById("Zusatzknopfe").style.visibility="hidden";
       document.getElementById("Einzelmuster").style.visibility="hidden";
    }
    }

   
  }
    if (e.data[0] == '$') {
    document.getElementById('Modusanzeige').value=e.data.substring(1);
    if(e.data[1] == '<'||e.data[1] == '>'||e.data[1] == '-'||e.data[1] == 'v'||e.data[1] == '^'){
       document.getElementById("Hz").style.visibility="visible";
         document.getElementById("Zusatzknopfe").style.visibility="visible";
       document.getElementById("Einzelmuster").style.visibility="visible";
       document.getElementById("BK").style.visibility="hidden";}
       if(e.data[1] == 'A'){
        document.getElementById("Hz").style.visibility="hidden";
          document.getElementById("Zusatzknopfe").style.visibility="hidden";
       document.getElementById("Einzelmuster").style.visibility="hidden";
       document.getElementById("BK").style.visibility="visible";
      }
       if(e.data[1] == 'U'){
       document.getElementById("Hz").style.visibility="hidden";
       document.getElementById("Zusatzknopfe").style.visibility="hidden";
       document.getElementById("Einzelmuster").style.visibility="hidden";
         document.getElementById("BK").style.visibility="hidden";
      }
  }

}
function Frequentierer(){
  var FQZ = parseInt(document.getElementById("Hz").value).toString(10);
  var FQZ = "!F" + FQZ;
console.log("Frequenz: " + FQZ);
connection.send(FQZ);
  }
  function Blinken(){
    console.log("Blinken");
    connection.send("!Blink")
    }
 function Brightness(){
    var He = parseInt(document.getElementById("Bright").value).toString(10);
  var He= "!H" + He;
console.log("Helligkeit: " + He);
connection.send(He);
    }
function sendButton() {
  console.log("Button: " );
  connection.send("!Modus");
}
function Musterbearbeitung(){
    console.log("Muster: " );
  connection.send("!pattern");
  }
function toggleVisibility() {
document.getElementById("Farben").style.display = "";
if(document.getElementById("Farben").style.visibility == "hidden" ) {
    document.getElementById("Farben").style.visibility = "visible";
    document.getElementById("Farben Einzeln").style.visibility = "hidden";
  
}
else {
document.getElementById("Farben").style.visibility = "hidden";
document.getElementById("Farben Einzeln").style.visibility = "visible";

}
}
function sendRGBg() {
var rg = parseInt(document.getElementById("rg").value).toString(16);
var gg = parseInt(document.getElementById("gg").value).toString(16);
var bg = parseInt(document.getElementById("bg").value).toString(16);
if (rg.length < 2) {
rg = "0" + rg;
}
if (gg.length < 2) {
gg = "0" + gg;
}
if (bg.length < 2) {
bg = "0" + bg;
}
var rgbg = "#g" + rg + gg + bg;
console.log("RGBg: " + rgbg);
connection.send(rgbg);
}

function sendRGB1() {
var r1 = parseInt(document.getElementById("r1").value).toString(16);
var g1 = parseInt(document.getElementById("g1").value).toString(16);
var b1 = parseInt(document.getElementById("b1").value).toString(16);
if (r1.length < 2) {
r1 = "0" + r1;
}
if (g1.length < 2) {
g1 = "0" + g1;
}
if (b1.length < 2) {
b1 = "0" + b1;
}
var rgb1 = "#1" + r1 + g1 + b1;
console.log("RGB1: " + rgb1);
connection.send(rgb1);
}

function sendRGB2() {
var r2 = parseInt(document.getElementById("r2").value).toString(16);
var g2 = parseInt(document.getElementById("g2").value).toString(16);
var b2 = parseInt(document.getElementById("b2").value).toString(16);
if (r2.length < 2) {
r2 = "0" + r2;
}
if (g2.length < 2) {
g2 = "0" + g2;
}
if (b2.length < 2) {
b2 = "0" + b2;
}
var rgb2 = "#2" + r2 + g2 + b2;
console.log("RGB2: " + rgb2);
connection.send(rgb2);
}
function sendRGB3() {
var r3 = parseInt(document.getElementById("r3").value).toString(16);
var g3 = parseInt(document.getElementById("g3").value).toString(16);
var b3 = parseInt(document.getElementById("b3").value).toString(16);
if (r3.length < 2) {
r3 = "0" + r3;
}
if (g3.length < 2) {
g3 = "0" + g3;
}
if (b3.length < 2) {
b3 = "0" + b3;
}
var rgb3 = "#3" + r3 + g3 + b3;
console.log("RGB3: " + rgb3);
connection.send(rgb3);

}
</script>
</head>
<link rel="stylesheet" href="/style.css">
<body style ="background-image: url(https://data.1freewallpapers.com/detail/love-heart.jpg);  background-size: cover;
  background-repeat: no-repeat;position: relative">
  <div class="Regler"style="float:right">
<div id="Farben" style="visibility:hidden;">
 Rot Gesamt :&nbsp&nbsp&nbsp<input id="rg" type="range" min="0" max="255" step="1" oninput="sendRGBg();" /><br>
 Gruen Gesamt:<input id="gg" type="range" min="0" max="255" step="1" oninput="sendRGBg();" /><br>
 Blau Gesamt :&nbsp&nbsp<input id="bg" type="range" min="0" max="255" step="1" oninput="sendRGBg();" />
</div>
<div id="Farben Einzeln" style="visibility:visible;">
Rot 1:&nbsp&nbsp&nbsp<input id="r1" type="range" min="0" max="255" step="1" oninput="sendRGB1();" /><br>
Gruen1:<input id="g1" type="range" min="0" max="255" step="1" oninput="sendRGB1();" /><br>
Blau 1:&nbsp&nbsp<input id="b1" type="range" min="0" max="255" step="1" oninput="sendRGB1();"/><br><br>

Rot 2:&nbsp&nbsp&nbsp<input id="r2" type="range" min="0" max="255" step="1" oninput="sendRGB2();"/><br>
Gruen2:<input id="g2" type="range" min="0" max="255" step="1" oninput="sendRGB2();"/><br>
Blau 2:&nbsp&nbsp<input id="b2" type="range" min="0" max="255" step="1" oninput="sendRGB2();"/><br><br>

Rot 3:&nbsp&nbsp&nbsp<input id="r3" type="range" min="0" max="255" step="1" oninput="sendRGB3();"/><br>
Gruen3:<input id="g3" type="range" min="0" max="255" step="1" oninput="sendRGB3();"/><br>
Blau 3:&nbsp&nbsp<input id="b3" type="range" min="0" max="255" step="1" oninput="sendRGB3();"/>
</div>
</div>
<div id="links" style="float: left"> 
Modus umschalten:<input id="Hauptknopf" type="button" value="Alle" onclick="sendButton();" ><br><br><br>
<input id="SyncronKnopf" type="button" value="Farbsyncronisation umschalten" onclick="toggleVisibility();" ><br><br><br>
Aktueller Modus:&nbsp&nbsp<input id="Modusanzeige" type="button" value="Uhr"  ><br><br><br>
<div id="Zusatzknopfe" style="visibility:hidden;">
<input  id="Einzelmuster" type="button" value="Muster" onclick="Musterbearbeitung();" ><br><br><br>
Frequenz: &nbsp&nbsp<input id="Hz" type="range" min="1" max="100" step="1" oninput="Frequentierer();" /><br><br><br>
<input id="BK" type="button" value="Blinken" onclick="Blinken();"style="visibility:hidden;" /><br><br><br>
</div>
Achtung die LED sind wirklich sehr hell!!!!!<br>

<--Dieser Wert muss geändert werden um Helligkeit zu regeln --!>
Helligkeit <input  id="Bright"  type="range" min="1" max="50" step="0.2" oninput="Brightness();" >
</div>
</body>
</html>
)rawliteral";

<html>
<head>

<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<style type="text/css">
body {
	font-family: sans-serif;
	font-size: 16px;
}

ul {
    padding: 0;
    margin: 0;
}

ul li a {
    display:block;
    padding: 8px 16px;
    color: #999;
    background-color: #e5e5e5;
    border-top: 1px solid #999;
    text-decoration: none;
    
}

ul li:first-child {
    border: none;
}

ul li a:hover {
    background-color: #f2f2f2;
    color: black;
}

.button {
	border-radius: 8px;
	padding: 8px;
	min-width: 100px;
}

.black {
	background-color: black;
	color: white;
}

.gray {
	background-color: #a0a0a0;
	color: black;
}

.blue {
	background-color: #008CBA;
	color: white;
}

.red {
	background-color: #f44336;
	color: white;
}

.small {
	font-size: 12px;
}

#header {
	padding-left: 16px;
	padding-bottom: 16px;
}

#header > h1 {
	padding-top: 16px;
	margin-bottom: 16px;
}

#outline {
	display: flex;
	justify-content: flex-start;
}

#left {
	width: 200px;
	min-width: 200px;
	max-width: 200px;
	flex: 200px;
}

#content {
	flex: 100%;
	padding: 16px;
}

.form {
	width: 600px;
}

form th {
	text-align: left;
	width: 200px;
}

.buttons {
	margin-top: 20px;
	padding-left: 200px;
}
</style>
<title>{{ app_name }}</title>

</head>
<body>
	<div id="header" class="black">
		<h1>{{ app_name }}</h1>
		<div class="small">version: {{ app_version }}, built at {{ build_date }}</div>
	</div>
	<div id="outline">
		<div id="left">
			<ul>
      			<li><a class="blue" href="/">Effects</a></li>
				<li><a class="blue" href="/wifi">WiFi settings</a></li>
				<li><a class="blue" href="/leds">LEDs settings</a></li>
				<li><a class="red" href="/reset">Reset to defaults</a></li>
			</ul>
		</div>
		<div id="content">

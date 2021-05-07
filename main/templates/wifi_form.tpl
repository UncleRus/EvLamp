<div class="form">
<form action="" method="post">

<div id="form_wifi">
<h3>WiFi settings</h3>
	<table>
	<tr>
		<th>Mode</th>
		<td>
			<select name="wifi_mode">
			{{#wifi.mode}}
				<option value="{{id}}" {{#selected}}selected{{/selected}}>{{name}}</option>
			{{/wifi.mode}}
			</select>
		</td>
	</tr>
	<tr>
		<th>Enable DHCP</th><td><input type="checkbox" name="wifi_ip_dhcp" {{#wifi.ip.dhcp}}checked{{/wifi.ip.dhcp}}></td>
	</tr>
	<tr>
		<th>IP</th><td><input type="text" name="wifi_ip_ip" value="{{wifi.ip.ip}}"></td>
	</tr>
	<tr>
		<th>Netmask</th><td><input type="text" name="wifi_ip_netmask" value="{{wifi.ip.netmask}}"></td>
	</tr>
	<tr>
		<th>Gateway</th><td><input type="text" name="wifi_ip_gateway" value="{{wifi.ip.gateway}}"></td>
	</tr>
	<tr>
		<th>DNS</th><td><input type="text" name="wifi_ip_dns" value="{{wifi.ip.dns}}"></td>
	</tr>
	</table>
</div>

<div id="form_wifi_ap">
<h3>Access Point settings</h3>
	<table>
	<tr>
		<th>SSID</th><td><input type="text" name="wifi_ap_ssid" value="{{wifi.ap.ssid}}"></td>
	</tr>
	<tr>
		<th>Channel</th><td><input type="number" name="wifi_ap_channel" value="{{wifi.ap.channel}}"></td>
	</tr>
	<tr>
		<th>Password</th><td><input type="text" name="wifi_ap_password" value="{{wifi.ap.password}}"></td>
	</tr>
	</table>
</div>

<div id="form_wifi_sta">
<h3>Station settings</h3>
	<table>
	<tr>
		<th>SSID</th><td><input type="text" name="wifi_sta_ssid" value="{{wifi.sta.ssid}}"></td>
	</tr>
	<tr>
		<th>Password</th><td><input type="text" name="wifi_sta_password" value="{{wifi.sta.password}}"></td>
	</tr>
	</table>
</div>

<div class="buttons">
	<input type="submit" class="button blue" value="Save">
</div>

</form>
</div>

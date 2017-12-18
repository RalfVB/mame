<?xml version="1.0" encoding="UTF-8"?>
<!-- license:BSD-3-Clause -->
<!-- copyright-holders:Joakim Larsson Edstrom -->

<!-- This XSLT converter has been tested on
Win-10: Chrome 61.0.3163.100 (Official Build) (64-bit), Firefox Version 56.0 (64-bitars), IE Edge 38.14393.1066.0.
Linux :
OSX   :
Compatibility:                   Win10: IE    Firefox  Chrome Linux:                      OSX: 
===================================================================================================================
HTML/SVG file generated by xsltproc     OK    OK       OK
Transformation browing the .LAY file    OK    Nope     Security
Transformation HTTP serving .LAY file   OK    Nope     OK
===================================================================================================================
-->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <!-- From https://stackoverflow.com/questions/22905134/convert-a-hexadecimal-number-to-an-integer-in-xslt -->
  <xsl:template name="decToHex">
    <xsl:param name="dec"/>
    <xsl:if test="$dec > 0">
      <xsl:call-template name="decToHex">
        <xsl:with-param name="dec" select="floor($dec div 16)"/>
      </xsl:call-template>
      <xsl:value-of select="substring('0123456789ABCDEF', (($dec mod 16) + 1), 1)"/>
    </xsl:if>
  </xsl:template>
  
  <!-- Handling of conversion of 0 as we know it should generate a two 0-digit sequence -->
  <xsl:template name="decimalToHex">
    <xsl:param name="dec"/>
    <xsl:if test="$dec > 0">
      <xsl:call-template name="decToHex">
        <xsl:with-param name="dec" select="$dec"/>
      </xsl:call-template>
    </xsl:if>
    <xsl:if test="$dec = 0">00</xsl:if>
  </xsl:template>

  <!-- Generate a 6 hex digit representation from a .LAY <color> definition --> 
  <xsl:template match="color">
    <xsl:param name="factor"/>
    <xsl:call-template name="decimalToHex">
      <xsl:with-param name="dec"><xsl:value-of select="floor(@red * 255 * $factor)"/></xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="decimalToHex">
      <xsl:with-param name="dec"><xsl:value-of select="floor(@green * 255 * $factor)"/></xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="decimalToHex">
      <xsl:with-param name="dec"><xsl:value-of select="floor(@blue * 255 * $factor)"/></xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:key name="getelement" match="mamelayout/element" use="@name"/>

  <xsl:template match="/">
    <xsl:variable name="this" select="."/>
    <html>
      <head>
	<title>Title</title>
	<script language="javascript">
	  <![CDATA[
	  var defstate = {};
	  var ws;
	  function Message(tag, mask, event)
	  {
	    var msg = { };
	    msg.inputtag = tag;
	    msg.inputmask = mask;
	    msg.event = event;
	    ws.send(JSON.stringify(msg));
	  }
	    
          function LayoutConnection()
          {
            if ("WebSocket" in window)
            {
               ws = new WebSocket("ws://localhost:8080/socket");
				
               ws.onopen = function() { console.log("Websocket open"); };

               ws.onmessage = function (evt) 
               { 
                 var msg = evt.data;
	         console.log("Websocket message: " + msg);
		 var dom = JSON.parse(msg);
		 if (dom.name == 'digit')
		 {
		   var digit = document.getElementById("digit" + dom.number).childNodes;
		   for (var index = 0; index < digit.length; index++)
		   {
		     if (dom.segments & (1 << index))
		       digit[index].setAttribute("fill", element_states["digit"][1]);
		     else
		       digit[index].setAttribute("fill", element_states["digit"][0]);
		   }
		 }
		   
	       };

               ws.onclose = function() {console.log("Websocket connection closed"); alert("Websocket connection closed!"); };

               window.onbeforeunload = function(event) { socket.close(); };
            }            
            else
            {
               alert("Your Browser doesn't support websockets!");
            }
            }
	    
	    function PageStart()
	    {
	    <!-- TODO: Loop through the default state array and set up all bezels using each element -->
	    console.log("Defstate: " + defstate);
	      for (var key in defstate) {
	      	console.log("Bezel: " + key + "State:" + defstate[key]);
	      }
	      <!-- Setup the websocket connection -->
	      LayoutConnection();
	    }
	  ]]>
	</script>
	<script>

	  <!-- Build default state array -->
	  <xsl:for-each select="mamelayout/element">
	    <xsl:if test="@defstate">
	      defstate['<xsl:value-of select="@name"/>'] =  <xsl:value-of select="@defstate"/>;
	    </xsl:if>
	  </xsl:for-each>

	  var element_states = {
	  <!-- Build element state array -->
	  <xsl:for-each select="mamelayout/element">
	    <xsl:variable name="name"><xsl:value-of select="@name"/></xsl:variable>
	    <xsl:value-of select="$name"/>: {
	    <xsl:for-each select="*">
	      <xsl:variable name="elem"><xsl:value-of select="name()"/></xsl:variable>
	      <xsl:if test="@state"><xsl:value-of select="@state"/>: "#<xsl:apply-templates select="color"><xsl:with-param name="factor">1.0</xsl:with-param></xsl:apply-templates>",</xsl:if>
	      <xsl:if test="not(@state)">1: "#<xsl:apply-templates select="color"><xsl:with-param name="factor">1.0</xsl:with-param></xsl:apply-templates>",</xsl:if>

	      <!-- segment unlit handling. rendlay.cpp defines a 0x202020 mask for led segements that are turned off, eg an implicit state="0" -->
	      <xsl:if test="$elem = 'led7seg'"> 
		<xsl:value-of select="@state"/>0: "#<xsl:apply-templates select="color"><xsl:with-param name="factor">0.13</xsl:with-param></xsl:apply-templates>", 
	      </xsl:if>

	    </xsl:for-each> },
	  </xsl:for-each> };
	</script>
      </head>
      <body onload="PageStart();"> 
	<xsl:for-each select="mamelayout/view">
	  <svg width="100%" height="100%" viewport="{bounds/@left} {bounds/@top} {bounds/@right} {bounds/@bottom}">
	    <xsl:for-each select="bezel">
	      <xsl:variable name="and"><![CDATA[&]]></xsl:variable>
	      <xsl:variable name="element"><xsl:value-of select="@element"/></xsl:variable>
	      <xsl:variable name="name"><xsl:value-of select="@name"/></xsl:variable>
	      <xsl:variable name="inputtag"><xsl:value-of select="@inputtag"/></xsl:variable>
	      <xsl:variable name="inputmask"><xsl:value-of select="@inputmask"/></xsl:variable>
	      <xsl:variable name="inputtag_onmousedown"><xsl:if test="@inputtag">Message('<xsl:value-of select="@inputtag"/>', <xsl:value-of select="@inputmask"/>, 0);</xsl:if></xsl:variable>
	      <xsl:variable name="inputtag_onmouseup"><xsl:if test="@inputtag">Message('<xsl:value-of select="@inputtag"/>', <xsl:value-of select="@inputmask"/>, 1);</xsl:if></xsl:variable>
	      <xsl:variable name="bounds_x"><xsl:value-of select="bounds/@x"/></xsl:variable>
	      <xsl:variable name="bounds_y"><xsl:value-of select="bounds/@y"/></xsl:variable>
	      <xsl:variable name="bounds_width"><xsl:value-of select="bounds/@width"/></xsl:variable>
	      <xsl:variable name="bounds_height"><xsl:value-of select="bounds/@height"/></xsl:variable>
	      <xsl:for-each select="key('getelement', $element)">
		<xsl:choose>
		  <xsl:when test="rect">
		    <rect x="{$bounds_x}"
			  y="{$bounds_y}"
			  width="{$bounds_width}"
			  height="{$bounds_height}"
			  style="fill:rgb({floor(rect/color/@red * 255)},{floor(rect/color/@green * 255)},{floor(rect/color/@blue * 255)});stroke-width:0"
			  onmousedown="{$inputtag_onmousedown}"
			  onmouseup="{$inputtag_onmouseup}"
			  />
		  </xsl:when>
		  <xsl:when test="text"><!-- TODO: Fix offsets, font and font size, .LAY seems to position from a centerpoint while SVG does it from left side. Needs investigation -->
		    <text x="{floor($bounds_x + $bounds_width * 0.25)}"
			  y="{floor($bounds_y + $bounds_height * 0.8)}"
			  font-family="sans-serif"
			  font-size="{floor($bounds_height * 0.75)}"
			  style="fill:rgb({floor(text/color/@red * 255)},{floor(text/color/@green * 255)},{floor(text/color/@blue * 255)});stroke-width:0.2;stroke:black"><xsl:value-of select="text/@string"/></text>
		  </xsl:when>
		  <xsl:when test="disk"><!-- TODO: Fix state support -->
		    <xsl:variable name="defstate"><xsl:value-of select="@defstate"/></xsl:variable>
		    <xsl:for-each select="disk">
		      <!-- TODO: store state colors in an array for each disk element -->
		    </xsl:for-each>
		    <!-- TODO: use the color index $defstate  -->
		    <ellipse cx="{floor($bounds_x + $bounds_width * 0.5)}"
			     cy="{floor($bounds_y + $bounds_height * 0.5)}"
			     rx="{floor($bounds_width * 0.5)}"
			     ry="{floor($bounds_height * 0.5)}"
			     style="fill:rgb({floor(disk/color/@red * 255)},{floor(disk/color/@green * 255)},{floor(disk/color/@blue * 255)});stroke-width:0.2;stroke:black"
			     onmousedown="{$inputtag_onmousedown}"
			     onmouseup="{$inputtag_onmouseup}"
			     />
		  </xsl:when>
		  <xsl:when test="led7seg">
		    <g id="{$name}" style="fill-rule:evenodd; stroke:#000000; stroke-width:0.25; stroke-opacity:1; stroke-linecap:butt; stroke-linejoin:miter;"
		       transform="translate(0,17) skewX(-4) translate(17,-17) translate({$bounds_x} {$bounds_y}) scale({$bounds_width div 11},{$bounds_height div 17}) ">

		        <!-- This definition is copied from Public Domain source: https://commons.wikimedia.org/wiki/File:7-segment_bcd.svg -->
			<!-- TODO: Add decimal point -->
			<polygon id="a" points=" 1, 1  2, 0  8, 0  9, 1  8, 2  2, 2" fill=""/>
			<polygon id="b" points=" 9, 1 10, 2 10, 8  9, 9  8, 8  8, 2" fill=""/>
			<polygon id="c" points=" 9, 9 10,10 10,16  9,17  8,16  8,10" fill=""/>
			<polygon id="d" points=" 9,17  8,18  2,18  1,17  2,16  8,16" fill=""/>
			<polygon id="e" points=" 1,17  0,16  0,10  1, 9  2,10  2,16" fill=""/>
			<polygon id="f" points=" 1, 9  0, 8  0, 2  1, 1  2, 2  2, 8" fill=""/>
			<polygon id="g" points=" 1, 9  2, 8  8, 8  9, 9  8,10  2,10" fill=""/>
		    </g>
		    <!-- debug -->
		    <!-- polygon points="{$bounds_x + 0},             {$bounds_y + 0}
				     {$bounds_x + 0},             {$bounds_y + $bounds_height}
				     {$bounds_x + $bounds_width}, {$bounds_y + $bounds_height}
				     {$bounds_x + $bounds_width}, {$bounds_y + 0}"
			     style="stroke-width:1;stroke:white;fill:none" / -->
		    <!-- /debug -->
		  </xsl:when>
		</xsl:choose>
		<xsl:text>&#xa;</xsl:text>
	      </xsl:for-each> 
	    </xsl:for-each>
	  </svg>
	</xsl:for-each>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>
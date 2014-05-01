"use strict";

var KeyStore = function() {
}

KeyStore.prototype.sendMessage = function(url,message,callback)
{
  var xmlHttp = null;
  xmlHttp = new XMLHttpRequest();

  xmlHttp.open('POST', url, true);
  
  xmlHttp.onreadystatechange = function () {
    if (xmlHttp.readyState != 4) 
      return;

    if (callback)
      callback(xmlHttp.responseText);
  }
  xmlHttp.send(""+message);
}

KeyStore.prototype.getKeys = function(callback) {
  this.sendMessage("/cgi-bin/lewg.http.key","LIST\n", callback );
}

KeyStore.prototype.remove = function(key, callback) {
  this.sendMessage("cgi-bin/lewg.http.key","DELETE *"+key+"#\n", callback);
}

KeyStore.prototype.create = function(key, callback) {
  var data = ""
    + "NEW *"+key+"#\n\n"
    + "GET LIFECYCLE *"+key+"#\n\n"
    + "GET COMMENT *"+key+"#\n\n";

  this.sendMessage("cgi-bin/lewg.http.key", data, callback);
}

KeyStore.prototype.setKey = function(key, lifecycle, comment, callback) {
  var data  = 
    "SET LIFECYCLE *"+key+"#\n"
      +lifecycle
      + "\n\n"
    +"GET LIFECYCLE *"+key+"#\n\n"
    +"SET COMMENT *"+key+"#\n"
      +comment
      +"\n\n"
    +"GET COMMENT *"+key+"#\n"
    +"\n\n";

  this.sendMessage("/cgi-bin/lewg.http.key", data, callback);
}


var KeyStoreUI = function()
{
  this.keys = new KeyStore();
}

KeyStoreUI.prototype.loadKeys = function()
{
  var self = this;
  this.keys.getKeys(function(result) { self.onMessage(result)});

  $("#keys").empty();
}


KeyStoreUI.prototype.parseLifeCycle = function(items) {

  if (items[0].substr(0,10) != "LIFECYCLE ")
    return false;

  var key = items[0].substr(10).trim();

  items.shift();

  this.createItem(key);

  $("#KEY_"+key+"_DETAILS").show()
  
  $("#KEY_"+key+"_DETAILS .lifecycle").empty()

  while (items.length && (items[0].substr(0,2) == "  "))
    parseDateTime(key, items.shift().substr(2).trim());

  return true;
}

KeyStoreUI.prototype.parseComment = function(items) {

  if (items[0].substr(0,8) != "COMMENT ")
    return false;

  var key = items[0].substr(8).trim();
  var text = "";

  items.shift();

  while (items.length && (items[0].substr(0,2) == "  "))
    text += items.shift().substr(2)+"\n";

  this.createItem(key);

  $("#KEY_"+key+" .comment")
    .text("C:"+text)
    .data("comment", text);

  return true;
}

KeyStoreUI.prototype.onMessage = function(message) {
  
  var items = message.split("\n");
  
  while (items.length)
  {
    if (this.parseLifeCycle(items))
      continue;
      
    if (this.parseComment(items))
      continue;
      
    items.shift();
  }
  
  w2popup.close();
}


KeyStoreUI.prototype.createItem = function(key) {

  var self = this;
  
  if ($("#KEY_"+key).length)
    return;
  
  $("#keys").
    append($('<div/>')
      .attr("id","KEY_"+key)
      .append($('<div/>')
        .append($('<span/>')
          .addClass("key")
          .text(key))
        .append($('<button/>')
          .text("delete")
          .click(function() { self.removeKey(key) } ))
        .append($('<button/>')
          .text("edit")
          .click(function() {
              showPopup(""+key);
            })))
      .append($('<div/>')
        .attr("id","KEY_"+key+"_DETAILS")
        .hide()
        .append($("<div/>")
          .addClass("comment"))
        .append($("<div/>")
          .addClass("lifecycle"))));
}

KeyStoreUI.prototype.removeKey = function(key) {
  var self = this;
  this.keys.remove(key, function(result) {self.onKeyRemoved(key, result)});
  
  $("#KEY_"+key).fadeTo(500,0.2);
}

KeyStoreUI.prototype.onKeyRemoved = function(key, result)
{
  $("#KEY_"+key).remove();
}

KeyStoreUI.prototype.createKey = function(key) {
  var self = this;
  this.keys.create(key, function(result) {  self.onMessage(result); showPopup(key); });

//  this.createItem(key);

//  $("#keys > div")
//    .sort(function(a,b) {
//        return ($(a).find(".key").text() > $(b).find(".key").text());
//      })
//    .appendTo("#keys");

//  $("#KEY_"+key)
//    .fadeTo(500,0.2);
}

//KeyStoreUI.prototype.onKeyCreated = function(key, result)
//{
//  $("#KEY_"+key)
//    .fadeTo(500,1);
//}

function parseDateTime(key,str)
{
    var str = $.trim(str);

    if (!str)
      return;

    var item = {};
    item.start = {};
    item.end = {};
    
    str = str.split(" UNTIL ");
    str[0] = str[0].split(" ");
    str[0][0] = str[0][0].split("-");
    str[0][1] = str[0][1].split(":");

    str[1] = str[1].split(" ");
    str[1][0] = str[1][0].split("-");
    str[1][1] = str[1][1].split(":");
    
    item.start.date = str[0][0];
    item.start.time = str[0][1];

    item.end.date = str[1][0];
    item.end.time = str[1][1];
    
    item.zone = str[1][2];
    
//    alert(key +" "+ item);

    //var date = new Date(item[0][0], item[0][1]-1, item[0][2]-1, item[1][0], item[1][1], 0);
    $("#KEY_"+key+" .lifecycle")
      .append($("<div/>")
        .addClass("lifecycleitem")
        .data("lifecycle", item)
        .append($("<span/>")
          .append(item.start.date[0]+"."+item.start.date[1]+"."+item.start.date[2]+" "+item.start.time[0]+":"+item.start.time[1]))
        .append($("<span/>").text(" until "))
        .append($("<span/>")
          .append(item.end.date[0]+"."+item.end.date[1]+"."+item.end.date[2]+" "+item.end.time[0]+":"+item.end.time[1])));
}


function createKey() {
  var key = document.getElementById("key").value;
  keys.create(key);
}

function deleteKey() {
  var key = document.getElementById("key").value;
  keys.remove(key);
}


  function extractDate(elm) {
    // get the start date
    var item = {};
    item.start = {};
    item.start.date = $('input[name="startDate"]',elm).val() || null;
    item.start.time = $('input[name="startTime"]',elm).val() || null;

    item.end = {};
    item.end.date = $('input[name="endDate"]',elm).val() || null;
    item.end.time = $('input[name="endTime"]',elm).val() || null;

    item.zone = $('select[name="zone"]',elm).val().toUpperCase() || null;
    
    if ((!item.start.date) && (!item.start.time) 
         && (!item.end.date) && (!item.end.time) && (!item.zone))
      return null;

    // Convert date...
    if (!item.start.date || !item.start.date.length)
      throw "Invalid start date";

    item.start.date = item.start.date.split(".");

    if (!item.end.date || !item.end.date.length)
      throw "Invalid end date";

    item.end.date = item.end.date.split(".");

    // Convert time...
    if (!item.start.time || !item.start.time.length)
      throw "Invalid start time";

    item.start.time = item.start.time.split(":");

    if (!item.end.time || !item.end.time.length)
      throw "Invalid end time";

    item.end.time = item.end.time.split(":");

    if (!item.zone || (item.zone != "(LOCAL)" && item.zone != "(GMT)"))
      throw "Invalid zone";

    return item;
  }

  KeyStoreUI.prototype.updateKey = function(key) {
  
    var result = "";

    try {
      $("#w2ui-popup .lewgList .lifecycle").each( function (index) {
      
        var item = extractDate(this);
        
        if (!item)
          return;
        
        // Put everything back again
        result += "  "
        result += item.start.date[2]+"-"+item.start.date[1]+"-"+item.start.date[0];
        result += (item.start.time[0].length == 1)?" 0":" ";
        result += item.start.time[0]+":"+item.start.time[1]+" UNTIL ";
      
        result += item.end.date[2]+"-"+item.end.date[1]+"-"+item.end.date[0];
        result += (item.end.time[0].length == 1)?" 0":" ";
        result += item.end.time[0]+":"+item.end.time[1];
        
        if (item.zone)
          result += " "+item.zone.toUpperCase();
        
        result += "\n";
      });
      
          var comment = $("#w2ui-popup textarea[name='comments']").val()

      // Normalize all linebreaks...
      comment = comment.replace(/\r\n|\r|\n|\u0085|\u000C|\u2028|\u2029/g,"\n")
    
      // Split and append spaces for each line...
      comment = comment.split("\n");
    
      var str = "";
      for (var i=0; i<comment.length; i++)
        str += "  "+comment[i]+"\n"
    }
    catch (e)
    {
      w2popup.message({ 
        html: '<div style="padding: 20px; text-align: center">'+e.toString()+'</div>', 
        width: 300, 
        height: 60,
        hideOnClick: true 
      });
      
      return;
    }
    
    w2popup.lock("Saving...",true);
    
    var self = this;
    this.keys.setKey(key,result,str, function(message) { self.onMessage(message); });
  }

  function createInput(item) {
  
    $("#w2ui-popup .lewgList")
      .append($("<div/>")
        .addClass("lifecycle")
        .append($("<input/>")
          .val((item)?(item.start.date[2]+"."+item.start.date[1]+"."+item.start.date[0]):"")
          .attr("name","startDate")
          .w2field('date',{ format: 'dd.mm.yyyy' }))
        .append($("<input/>")
          .attr("name","startTime")
          .val((item)?(item.start.time[0]+":"+item.start.time[1]):"")
          .w2field('time',{ format: 'h24' }))
        .append($("<span/>")
          .text(" until "))
        .append($("<input/>")
          .val((item)?(item.end.date[2]+"."+item.end.date[1]+"."+item.end.date[0]):"")
          .attr("name","endDate")
          .w2field('date',{ format: 'dd.mm.yyyy' }))
        .append($("<input/>")
          .val((item)?(item.end.time[0]+":"+item.end.time[1]):"")
          .attr("name","endTime")
          .w2field('time',{ format: 'h24' }))
        .append($("<select/>")
          .attr("name","zone")
          .append($("<option/>")
            .text(""))
          .append($("<option/>")
            .text("Local")
            .val("(LOCAL)"))
          .append($("<option/>")
            .text("GMT")
            .val("(GMT)"))
          .val((item)?item.zone:""))
        .append($("<button/>")
          .text("Delete")
          .click(function() { $(this).parent().remove();})));
  }

  function swapDate(item)
  {
    var tmp = item.date[0];
    item.date[0] = item.date[2];
    item.date[2] = tmp;

    return item;
  }

  function createInput2() {
   if ($("#w2ui-popup .lewgList .lifecycle").size()  <= 1)
     createInput();
    
    $("#w2ui-popup .lewgList2")
      .append($("<button/>")
        .text("Add")
        .click(function() { createInput(); })
      );
  }

  function showPopup(key) {
  
    // TODO check if key exists:
    
    
    // ... the the popup
    $("#popup1").w2popup({
      onOpen : function(event) {
        event.onComplete = function() {
          $("#w2ui-popup .lewgList").empty();
          
          $("#KEY_"+key+" .lifecycleitem").each(function(index) {
            createInput($(this).data("lifecycle"));
          });

          $("#w2ui-popup textarea[name='comments']").val($("#KEY_"+key+" .comment").data("comment"));
          
          createInput2();
          
          $("#w2ui-popup .w2ui-msg-buttons input").click( function() {
            keyUI.updateKey(key);
          })
        }
      }
    });
  }



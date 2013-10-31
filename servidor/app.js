//Collection
Data = new Meteor.Collection('Data');

//Routing
Router.configure({
  autoRender: false
});

Router.map(function() { 
  this.route('home', {path: '/'});
  this.route('about');
  this.route('data',{
    where: 'server',
    path: '/log/:_temp/:_hum/:_prs',
    action: function(){
      var hor = Date.now();
      var temp = this.params._temp;
      var hum = this.params._hum;
      var prs = this.params._prs;
      Data.insert({hora: hor,temperatura: temp, humedad: hum, presion: prs});
    }
  })
});

if (Meteor.isClient) {

 /* Template.hello.events({
    'click input' : function () {
      // template data, if any, is available in 'this'
      if (typeof console !== 'undefined')
        console.log("You pressed the button");
    }
  });*/

  Template.home.allData = function(){
    return Data.find({}, {sort: {hora: -1}});
  }

  Template.home.helpers({
    niceTime: function () {
      // convert UTC timestamp to something normal people can reads
      return new Date(this.hora);
    }
  });
}

if (Meteor.isServer) {
  Meteor.startup(function () {
    // code to run on server at startup
  });
}

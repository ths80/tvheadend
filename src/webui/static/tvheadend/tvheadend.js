tvh.api = '/api/v1/'

/*

tvh.DevTree = Ext.extend(Ext.tree.TreePanel, {
  initComponent:function() {

    Ext.apply(this, {
      useArrows: true,
      autoScroll: true,
      animate: true,
      containerScroll: true,
      dataUrl: 'api/extjs/devicetree',

      root: {
        nodeType: 'async',
        text: 'Devices',
        draggable: false,
        id: 'root',
      }
    });
    tvh.DevTree.superclass.initComponent.apply(this, arguments);
  },
 
  onRender:function() {
    tvh.DevTree.superclass.onRender.apply(this, arguments);
    this.getRootNode().expand();
  }
});
Ext.reg('tvhDevTree', tvh.DevTree);


tvh.deviceView = new Ext.Panel({
  title: 'Devices',
  layout: 'table',
  layoutConfig: {
        columns: 2
  },
  items: [{
    title: 'Devices',
    xtype: 'tvhDevTree',
    rowspan: 2
  }, {
    html: '<p>Data</p>'
  }, {
    html: '<p>Props</p>'
  }]
});
*/

Ext.reg('tvhDevicePanel', Ext.extend(Ext.Panel, {

  initComponent:function() {
    Ext.apply(this, {
      layout: 'table',
      height: 500,
      layoutConfig: {
        columns: 2
      },
      defaults: {frame:true, width:'auto', height: 'auto'},

      items: [{
	title: 'devices',
	xtype: 'treepanel',
	useArrows: true,
	autoScroll: true,
	animate: true,
	dataUrl: 'api/extjs/devicetree',
	
	root: {
          nodeType: 'async',
          text: 'Devices',
          draggable: false,
          id: 'root',
	}
      }, {
	text: 'press me',
	xtype: 'button'
      }]
    });
    this.constructor.superclass.initComponent.apply(this, arguments);
  },
  
  onRender:function() {
    this.constructor.superclass.onRender.apply(this, arguments);
  }
}));


Ext.onReady(function() {
  Ext.QuickTips.init();
      
  var viewport = new Ext.Viewport({
    layout:'fit',
    items:[{
      xtype: 'grouptabpanel',
      tabWidth: 130,
      activeGroup: 0,
      items: [{
    	items: [{
	  title: 'Devices',
	  xtype: 'tvhDevicePanel'
	}]
      }]
    }]
  });
});


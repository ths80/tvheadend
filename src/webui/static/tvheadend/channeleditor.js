tvh.channelEditor = Ext.extend(Ext.grid.GridPanel, {
  initComponent:function() {

    Ext.apply(this, {
      store:new Ext.data.Store({
	id: 'user',
	restful: true,
	proxy: new Ext.data.HttpProxy({
	  url: tvh.api + 'channels'
	}),

	reader: new Ext.data.JsonReader({
	  totalProperty: 'total',
	  successProperty: 'success',
	  idProperty: 'id',
	  root: 'data',
	  messageProperty: 'message'
	}, [
	  {name: 'id'},
	  {name: 'email', allowBlank: false},
	  {name: 'first', allowBlank: false},
	  {name: 'last', allowBlank: false}
	]),

	writer: new Ext.data.JsonWriter({
	  encode: false
	})
      }),

      columns:[
      {header: "ID", width: 40, sortable: true, dataIndex: 'id'},
      {header: "Email", width: 100, sortable: true, dataIndex: 'email', editor: new Ext.form.TextField({})},
      {header: "First", width: 50, sortable: true, dataIndex: 'first', editor: new Ext.form.TextField({})},
      {header: "Last", width: 50, sortable: true, dataIndex: 'last', editor: new Ext.form.TextField({})}
      ],
      frame: true,
      iconCls: 'icon-grid',
      viewConfig: {
        forceFit: true
      }

    });
 
    tvh.channelEditor.superclass.initComponent.apply(this, arguments);
  },
 
  onRender:function() {
    this.store.load();
    
    tvh.channelEditor.superclass.onRender.apply(this, arguments);
  }
});
Ext.reg('channelEditor', tvh.channelEditor);

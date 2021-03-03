// 这里记录的是图表各种默认配置，当用户在测试页面选择不同的选项时，也就是是选择了这里的某组配置参数。
export default {
  textIsShow:{
    type: 'boolean',
    title: '是否显示文本',
    default: true
  },
  textFontFamily: {
    type: 'enum',
    title: '文字字体',
    default: 'Arial',
    values: ['Arial','Helvetica','Verdana','Times','sans-serif'],
  },
  textFontColor: {
    type: 'color',
    title: '文字颜色',
    default: '#000',
  }
};

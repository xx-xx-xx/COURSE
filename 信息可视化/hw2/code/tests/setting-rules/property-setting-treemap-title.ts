// 这里记录的是图表各种默认配置，当用户在测试页面选择不同的选项时，也就是是选择了这里的某组配置参数。
export default {
  titleText: {
    type: 'string',
    title: '标题文本',
    default: 'TreeMap'
  },
  titleIsShow: {
      type: 'boolean',
      title: '是否显示标题',
      default: false
    },
  titleBackground: {
    type: 'color',
    title: '标题背景色',
    default: '#E3E3E3',
  },
  titlePosition: {
    type: 'enum',
    title: '标题位置',
    default: 'top',
    values: ['top','bottom','left','right'],
  },
  titleTextPosition: {
    type: 'enum',
    title: '文字对齐方式',
    default: 'center',
    values: ['left','center','right'],
  },
  titleFontSize: {
    type: 'number',
    title: '文字字号',
    default: 16,
    values: [10,50],
  },
  titleFontFamily: {
    type: 'enum',
    title: '文字字体',
    default: 'Arial',
    values: ['Arial','Helvetica','Verdana','Times','sans-serif'],
  },
  titleFontColor: {
    type: 'color',
    title: '文字颜色',
    default: '#000',
  }
};

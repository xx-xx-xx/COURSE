// 这里记录的是图表各种默认配置，当用户在测试页面选择不同的选项时，也就是是选择了这里的某组配置参数。
export default {
    legendIsShow: {
      type: 'boolean',
      title: '是否显示图例',
      default: true
    },
    legendPosition: {
      type: 'enum',
      title: '图例位置',
      default: 'right',
      values: ['top','bottom','left','right','left-top','right-top','left-bottom','right-bottom'],
    },
    legendAlign: {
      type: 'enum',
      title: '图例排列方式',
      default: 'vertical',
      values: ['vertical','horizontal'],
    },
  };
  
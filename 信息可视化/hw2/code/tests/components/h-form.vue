<template>
  <div class="comp-h-form">
    <el-form ref="form" v-bind="formSet" v-model="formData">
      <template v-for="(item, i) in items">
        <el-form-item
          v-if="item.formItem && item.formComp"
          :key="'form-item-'+ i"
          :label="item.formItem.label||''"
          :prop="item.formItem.prop"
        >
          <component
            v-if="item.formComp.is"
            v-bind="item.formComp"
            :key="'comp-' + i"
            :is="item.formComp.is"
            v-model="formData[item.formItem.prop]"
            v-on="buildEventCallback(item.events, item, item.formItem.prop)"
          >
            <template v-if="item.formComp.is === 'el-select'">
              <el-option v-for="(op,k) in (item.items || [])" :key="i + 'op' + k" v-bind="op"></el-option>
            </template>
          </component>
        </el-form-item>
      </template>
    </el-form>
  </div>
</template>
<script>
/**
 * form item eg：
 * {
 *    formItem: {
 *      label: '',
 *      prop: 'prop'
 *    },
 *    formComp: {
 *      is: 'el-input',
 *      placeholder: 'please ...'
 *    },
 *    events: {
 *      change: 'testInputChange'
 *    }
 *  }
 */
export default {
  props: {
    formSet: {
      type: Object,
      default () {
        return {
          size: 'small',
          inline: false,
          class: 'setting-form',
          labelWidth: '120px'
        }
      }
    },
    items: {
      type: Array,
      default () {
        return [
        ];
      }
    },
    formData: {
      type: Object,
      default () {
        return {}
      }
    }
  },
  methods: {
    buildEventCallback (eventMaps = {}, item, property) {
      const that = this;
      const on = {};
      Object.keys(eventMaps).forEach(event => {
        on[event] = (eventDefArgs) => {
          that.$emit('eventdone', { type: event, eventName: eventMaps[event], args: eventDefArgs, emitor: that, formItem: item, property: property })
        }
      })
      return on;
    },
    getForm () {
      return this.$refs.form;
    }
  }
}
</script>
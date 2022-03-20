<template>
  <div>
    <ReactiveBarChart
      class="chart"
      v-if="chartData"
      v-bind:chart-data="chartData"
    />
    <DataSendForm />
  </div>
</template>

<script>
import ReactiveBarChart from "../components/ReactiveBarChart";
// 입력 데이터 컴포넌트 사용
import DataSendForm from "../components/DataSendForm.vue";
// axios & vuex 사용
import { dataLab } from "../utils/axios";
import { mapState, mapMutations } from "vuex";
export default {
  name: "MainView",
  components: {
    ReactiveBarChart,
    DataSendForm
  },

  // 백엔드에서 데이터 가져와 필요한 데이터로 가공해 반환

  async created() {
    // 백엔드 JSON 데이터를 가져온다.
    const response = await dataLab.get();
    const chartData = {
      // res를 콘솔로 찍어서 필요한 데이터만 가져오기
      labels: response.data[0].data.map((li) => li.period),
      datasets: response.data.reduce((acc, cur) => {
        const label = cur.title;
        const data = cur.data.map((li) => li.ratio);
        const color = this.makeColor();
        acc.push({
          label: label,
          data: data,
          fill: false,  // 선만 표시
          backgroundColor: color, // 점 색깔
          borderColor: color, // 선 색깔
        });
        return acc;
      }, []),
    };
    // vuex로 변경
    this.CHANGE_CHART_DATA(chartData);
  },
  computed: {
    ...mapState(["chartData"]),
  },
  methods: {
    ...mapMutations(["CHANGE_CHART_DATA"]),
    makeColor() {
      // 랜덤 색깔 만들어주기!
      return "#" + Math.round(Math.random() * 0xffffff).toString(16);
    }
  },
};
</script>

<style scoped>
.chart {
  width: 500px;
}
</style>

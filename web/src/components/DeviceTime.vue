<template>
  <v-col cols="12">
    <v-card>
      <v-card-item>
        <v-card-title>Device Time</v-card-title>
      </v-card-item>
      <v-card-text>
        <p>Current time: {{ deviceTimeDisplay }}</p>
      </v-card-text>
      <v-card-actions>
        <v-btn
          prepend-icon="mdi-refresh"
          :loading="loading"
          @click="reloadDeviceTime"
        >
          Refresh
        </v-btn>
        <v-btn prepend-icon="mdi-history" @click="ntpUpdate">
          Force NTP Reload
        </v-btn>
      </v-card-actions>
    </v-card>
  </v-col>
</template>

<script>
const GET_TIME_URL = "/unixtime";
const REFRESH_TIME_URL = `${GET_TIME_URL}/refresh`;

export default {
  data: () => ({
    loading: true,
    deviceTime: null,
  }),
  methods: {
    async reloadDeviceTime() {
      this.loading = true;
      try {
        const resp = await fetch(GET_TIME_URL);
        const data = await resp.json();
        this.setTime(data);
      } catch (e) {
        console.error(e);
      } finally {
        this.loading = false;
      }
    },
    async ntpUpdate() {
      this.loading = true;
      try {
        const resp = await fetch(REFRESH_TIME_URL, { method: "POST" });
        const data = await resp.json();
        this.setTime(data);
      } catch (e) {
        console.error(e);
      } finally {
        this.loading = false;
      }
    },
    setTime({ unixtime, tz_offset }) {
      const time = new Date((unixtime - tz_offset) * 1000);
      this.deviceTime = time.toLocaleString();
    },
  },
  computed: {
    deviceTimeDisplay() {
      return this.loading ? "<fetching>" : this.deviceTime;
    },
  },
  mounted() {
    this.reloadDeviceTime();
  },
};
</script>

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
const API_URL = "http://worldtimeapi.org/api/timezone/America/New_York";

export default {
  data: () => ({
    loading: true,
    deviceTime: null,
  }),
  methods: {
    async reloadDeviceTime() {
      // For now we'll use an external API. Eventually we'll run a webserver on the
      // ESP.
      console.debug("TODO: implement device time route");
      this.loading = true;
      try {
        const resp = await fetch(API_URL);
        const data = await resp.json();
        const { unixtime } = data;
        const time = new Date(unixtime * 1000);
        this.deviceTime = time.toLocaleString();
      } catch (e) {
        console.error(e);
      } finally {
        this.loading = false;
      }
    },
    ntpUpdate() {
      console.debug("TODO: implement NTP update route");
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

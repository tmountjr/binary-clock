<template>
  <v-col cols="12" md="4">
    <v-card>
      <v-card-item>
        <v-card-title>Invert Display</v-card-title>
      </v-card-item>
      <v-card-text>
        <p>
          This option will invert the display (in case you want to place the
          clock upside down).
        </p>
        <p>Current device setting: {{ inversionStatus }}</p>
      </v-card-text>
      <v-card-actions>
        <v-btn
          prepend-icon="mdi-flip-vertical"
          :loading="loading"
          @click="invertDisplay"
        >
          Invert
        </v-btn>
      </v-card-actions>
    </v-card>
  </v-col>
</template>

<script>
const SET_INVERT_URL = "/display/invert";

export default {
  data: () => ({
    loading: false,
    binaryStatus: false,
  }),
  methods: {
    async invertDisplay() {
      try {
        const resp = await fetch(SET_INVERT_URL, {
          method: "POST",
          Accept: "application/json",
        });
        const data = await resp.json();
        this.binaryStatus = data.invertStatus;
      } catch (e) {
        console.error(e);
      }
    },
  },
  computed: {
    inversionStatus() {
      return this.binaryStatus ? "Inverted" : "Normal";
    },
  },
};
</script>

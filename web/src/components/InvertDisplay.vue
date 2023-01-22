<template>
  <v-col cols="12">
    <v-card>
      <v-card-item>
        <v-card-title>Invert Display</v-card-title>
      </v-card-item>
      <v-card-text>
        <p>
          This option will invert the display (in case you want to place the
          clock upside down).
        </p>
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
export default {
  data: () => ({
    loading: false,
  }),
  methods: {
    async invertDisplay() {
      console.debug("TODO: implement inversion route and functionality");
    },
  },
};
</script>

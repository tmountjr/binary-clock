import { createApp } from "vue";
import { createPinia } from "pinia";
import App from "./App.vue";

// Vuetify
import "vuetify/styles";
import { createVuetify } from "vuetify";
import * as components from "vuetify/components";
import * as directives from "vuetify/directives";
import { aliases, mdi } from "vuetify/iconsets/mdi";
const vuetify = createVuetify({
  icons: {
    defaultSet: "mdi",
    aliases,
    sets: { mdi },
  },
  components,
  directives,
});

import "./assets/main.css";

const app = createApp(App);

app.use(createPinia());
app.use(vuetify);
app.mount("#app");

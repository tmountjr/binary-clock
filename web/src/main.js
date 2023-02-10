import { createApp } from "vue";
import App from "./App.vue";

// Vuetify
import { createVuetify } from "vuetify";
const vuetify = createVuetify();

import "./assets/main.css";

const app = createApp(App);

app.use(vuetify);
app.mount("#app");

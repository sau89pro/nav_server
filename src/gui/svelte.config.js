import adapter from '@sveltejs/adapter-static';
import { vitePreprocess } from '@sveltejs/vite-plugin-svelte';

const buildPath = '../../build/bin';

/** @type {import('@sveltejs/kit').Config} */
const config = {
	// Consult https://svelte.dev/docs/kit/integrations
	// for more information about preprocessors
	preprocess: vitePreprocess(),

	kit: {
		adapter: adapter({
			pages: buildPath,
			assets: buildPath,
			fallback: undefined,
			precompress: false,
			strict: true
		}),
		files: {
			appTemplate: 'src/template.html'
		},
	},
};

export default config;

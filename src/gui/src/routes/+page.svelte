<script lang="ts">
	//import Nested from './Nested.svelte';

	const name = 'Svelte';
</script>

<div class="main">
	<p>Hello {name.toUpperCase()}!</p>
	<!-- <Nested /> -->
</div>

<style>
	.main {
		position: absolute;
		width: 100vw;
		height: 100vh;
		background-color: lightgreen;
	}

	p {
		color: goldenrod;
		font-family: 'Comic Sans MS', cursive;
		font-size: 2rem;
	}
</style>


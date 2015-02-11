#include <a_samp>
#include <sqlsort>

public OnFilterScriptInit()
{
	#define array_size 10

	new
		array[array_size],
		pre_tick,
		post_tick
	;

	for(new i; i < array_size; i ++)
		array[i] = random(1001) - random(1001);

    pre_tick = GetTickCount();
	sqlSort(array, .order = sqlSort_Descending); // Descending Order
//	sqlSort(array, .order = sqlSort_Ascending); // Ascending Order
	post_tick = GetTickCount();

	for(new i; i < array_size; i ++)
		printf("%i", array[i]);

	printf("sqlSort executed in %i ms", post_tick - pre_tick);
}

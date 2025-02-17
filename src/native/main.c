
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <mono/jit/mono-private-unstable.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>


static const char* trusted_assemblies[] = { 
#undef TRUSTED_PLATFORM_ASSEMBLY
#define TRUSTED_PLATFORM_ASSEMBLY(x) x,
#include "tpa.h"
"TRUSTED_PLATFORM_ASSEMBLY" // last random string because ' x,' has a surplus ' ,' which creates a syntax error
#undef TRUSTED_PLATFORM_ASSEMBLY
};
static int num_trusted_assemblies = (sizeof(trusted_assemblies) / sizeof(trusted_assemblies[0])) - 1;//minus the random string


#ifdef PREPROCESSORDEF_VISUAL_STUDIO
#include "wincoretpa.h"
#endif
//#else : Makefile def


#ifdef _MSC_VER
#define PATH_SEP_CHAR ';'
#else
#define PATH_SEP_CHAR ':'
#endif

/**
 * Make the trusted platform assemblies property for the runtime.
 * The TPA is a path-separator separated list of assembly filenames that the runtime will use to load assemblies.
 * The list should include System.Private.CoreLib as well as the needed base class library assemblies,
 * and any assemblies used by the managed app code itself.
 * 
 */
char* make_tpa_list (void)
{
	size_t len = 0;
	len += strlen(CORELIB_FULL_PATH);
	for (int i = 0; i < num_trusted_assemblies; ++i) {
		len += strlen (trusted_assemblies [i]);
	}
	len += num_trusted_assemblies; /* account for path separator chars between assemblies */
	char *buf = malloc (len + 1);
	char *ptr = buf;
	ptr += sprintf (ptr, "%s", CORELIB_FULL_PATH);
	for (int i = 0; i < num_trusted_assemblies; ++i) {
		ptr += sprintf (ptr, "%c%s", PATH_SEP_CHAR, trusted_assemblies[i]);
	}
	return buf;
}

/* This is a magic number that must be passed to mono_jit_init_version */
#define FRAMEWORK_VERSION "v4.0.30319"

int main(void)
{
	char *tpa_list = make_tpa_list ();
	const char *prop_keys[] = {"TRUSTED_PLATFORM_ASSEMBLIES"};
	char *prop_values[] = {tpa_list};
	int nprops = sizeof(prop_keys)/sizeof(prop_keys[0]);
	monovm_initialize (nprops, (const char**) &prop_keys, (const char**) &prop_values);
	free (tpa_list);
	MonoDomain *root_domain = mono_jit_init_version ("embedder_sample", FRAMEWORK_VERSION);

	if (!root_domain) {
		printf ("root domain was null, expected non-NULL on success\n");
		return 1;
	}
	printf ("runtime initialized\n");

	const char *sample_assm = "CsharpSample, Version=1.0.0.1"; /* can also specify Culture and PublicKeyToken */

	MonoAssemblyName *aname = mono_assembly_name_new (sample_assm);

	if (!aname) {
		printf ("Couldn't parse assembly name '%s'\n", sample_assm);
		return 1;
	}
	
	MonoImageOpenStatus status;
	MonoAssembly *main_sample_assm = mono_assembly_load_full (aname, /*basedir*/ NULL, &status, 0);

	if (!main_sample_assm || status != MONO_IMAGE_OK) {
		printf ("Couldn't open \"%s\", (status=0x%08x)\n", sample_assm, status);
		return 1;
	}
	mono_assembly_name_free (aname);
	aname = NULL;
		
	MonoImage *img = mono_assembly_get_image (main_sample_assm);

	MonoClass *kls = mono_class_from_name (img, "CsharpSample", "SampleClass");
	if (!kls) {
		printf ("Coudln't find CsharpSample.SampleClass in \"%s\"\n", sample_assm);
		return 1;
	}

	MonoMethod *create = mono_class_get_method_from_name (kls, "Create", 0);
	if (!create) {
		printf ("No Create method in CsharpSample.SampleClass\n");
		return 1;
	}

	void *args[1];
	MonoObject *exc;

	MonoObject *obj = mono_runtime_invoke (create, NULL, (void**)&args, NULL);

	MonoMethod *hello = mono_class_get_method_from_name (kls, "Hello", 0);

	if (!hello) {
		printf ("No Hello method in CsharpSample.SampleClass\n");
		return 1;
	}

	mono_runtime_invoke (hello, obj, (void**)&args, NULL);

	fflush (stdout);

	return 0;
}

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "Cute.h"

#include "CuteInstr.h"

#include "common/atom.h"
#include "common/config.h"
#include "common/error.h"

#include "objects/manager.h"
#include "utils/utils.h"

#include "core/core.h"
#include "core/context.h"
#include "core/contextdef.h"


void
ct_runtime_init(CtRuntime* runtime) {
	CT_LOG("runtime", "vroom vroom\n");
	runtime->exit_code = 0;
}


void
ct_runtime_end(CtRuntime* runtime) {

	if (runtime->image.header.magic_id == ct_magic_id) {
		CT_LOG("runtime", "Freeing image resources.\n");
		ct_image_free(&runtime->image);
	}

	CT_LOG("runtime", "Ending runtime.\n");
	exit(runtime->exit_code);
}


void
ct_runtime_load(CtRuntime* runtime, const char* filepath) {
	
	CT_LOG("runtime", "Loading image file: %s\n", filepath);

	CtImageStatus code = ct_image_load(&runtime->image, filepath);

	switch (code) {

		case CT_IMAGE_STATUS_SUCCESS:
			CT_LOG("runtime", "Image loaded successfully.\n");
			break;

			case CT_IMAGE_STATUS_FILE_NOT_FOUND:
			CT_ERROR_RUNTIME(
				runtime->error, 
				"Runtime", 
				"ImageNotFound", 
				"Cannot find image file: %s", filepath
			);
			break;

		case CT_IMAGE_STATUS_READ_WRITE_FAILURE:
			CT_ERROR_RUNTIME(
				runtime->error, 
				"Runtime", 
				"ImageReadWriteFailure", 
				"Failed to read image file: %s", filepath
			);
			break;

		case CT_IMAGE_STATUS_CORRUPTED_IMAGE:
			CT_ERROR_RUNTIME(
				runtime->error, 
				"Runtime", 
				"CorruptedImage", 
				"Invalid image file: %s", filepath
			);
			break;

		case CT_IMAGE_STATUS_VERSION_MISTMATCH:
			CT_ERROR_RUNTIME(
				runtime->error, 
				"Runtime", 
				"VersionMismatch", 
				"Version mismatch.", NULL
			);
			break;

		default:
			CT_ERROR_RUNTIME(
				runtime->error, 
				"Runtime", 
				"UnknownImageError", 
				"Unknown failure while reading image file: %s", filepath
			);
			break;
	}

	if (code != CT_IMAGE_STATUS_SUCCESS) {
		ct_error_print(&runtime->error);
		runtime->exit_code = 1;
		ct_runtime_end(runtime);
		exit(1);
	}
}


void
ct_runtime_run_context(CtRuntime* runtime, CtContext* ctx) {

	ct_runtime_exec(runtime, ctx);
	
	if (ct_thread_error.raised) {
		ct_error_print(&ct_thread_error);
	}

	runtime->exit_code = ctx->exit_code;
}

void
cute_run(int argc, char** argv) {

	if (argc <= 1) {
		printf("usage: cute <image>\n");
		exit(2);
	}

	CtRuntime runtime;
	ct_runtime_init(&runtime);
	ct_runtime_load(&runtime, argv[1]);

	
	ct_runtime_run_context(
		&runtime,
		ct_ctx_new(&runtime.image, ct_objects_init(), 0)
	);

	ct_runtime_end(&runtime);

}
import angr
from angr.knowledge_plugins.patches import PatchManager

def patch_nop(addr, bytes, pm):
    patch_data = b'\x90' * bytes
    pm.add_patch(addr, patch_data)

def bin_save(file_name, pm):
    patched_data = pm.apply_patches_to_binary()
    with open(file_name, "wb") as patched_file:
        patched_file.write(patched_data)


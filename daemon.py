import os
import time
import subprocess
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

# The file to watch
FILENAME = "kernel.bin"


# Function to run when the file changes
def run_qemu():
    print("kernel.bin changed. Running QEMU...")
    subprocess.run(
        [
            "qemu-system-i386",
            "-no-reboot",
            "-no-shutdown",
            "-device",
            "isa-debug-exit,iobase=0xf4,iosize=0x04",
            "-device",
            "i8042",
            "-drive",
            f"file={FILENAME},format=raw,index=0,if=floppy",
            "-boot",
            "a",
            "-vga",
            "std",
            "-m",
            "256M",
            "-k",
            "il",
        ]
    )


class KernelChangeHandler(FileSystemEventHandler):
    def __init__(self):
        self.last_modified = os.path.getmtime(FILENAME)

    def on_modified(self, event):
        if not event.is_directory and event.src_path.endswith(FILENAME):
            current_modified = os.path.getmtime(FILENAME)
            if current_modified != self.last_modified:
                self.last_modified = current_modified
                run_qemu()


if __name__ == "__main__":
    path = "."
    event_handler = KernelChangeHandler()
    observer = Observer()
    observer.schedule(event_handler, path, recursive=False)
    observer.start()
    print(f"Watching for changes to {FILENAME}...")
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()

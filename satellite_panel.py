import customtkinter as ctk
from tkinter import filedialog
import subprocess
import tempfile
import os

ctk.set_appearance_mode("dark")
ctk.set_default_color_theme("blue")

APP_SIZE = "1200x600"
APP_TITLE = "Data Compression Panel"
FONT = "Trebuchet MS"
LEFT_COLOR  = "#0f0f0f"
RIGHT_COLOR = "#141414"
PRIMARY_COLOR = "#b3b3b3"
FRAME_COLOR = "#1a1a1a"
BUTTON_TEXT_COLOR = "#1a1a1a"
BUTTON_HOVER_COLOR = "#FFFFFF"

BINARY_PATH = "./cpp/build/minibit_compressor"

METHOD_MAP = {
    "Run-Length Encoding": "rle",
    "Possible Combinations": "pce",
}

ALL_FILETYPES = [
    ("All Files",      "*.*"),
    ("Text Files",     "*.txt *.csv *.log *.md *.json *.xml *.html *.htm"),
    ("Image Files",    "*.png *.jpg *.jpeg *.bmp *.gif *.tiff *.webp"),
    ("Video Files",    "*.mp4 *.avi *.mkv *.mov *.wmv *.flv *.webm"),
    ("Audio Files",    "*.mp3 *.wav *.flac *.aac *.ogg *.m4a"),
    ("Archive Files",  "*.zip *.rar *.7z *.tar *.gz *.bz2"),
    ("Binary / Other", "*.bin *.dat *.exe *.dll *.iso *.rle"),
]

def format_size(size_bytes):
    if size_bytes < 1024:
        return f"{size_bytes} B"
    elif size_bytes < 1024 ** 2:
        return f"{size_bytes / 1024:.2f} KB"
    elif size_bytes < 1024 ** 3:
        return f"{size_bytes / (1024 ** 2):.2f} MB"
    else:
        return f"{size_bytes / (1024 ** 3):.2f} GB"

def show_error(app, message):
    popup = ctk.CTkToplevel(app)
    popup.title("Error")
    popup.geometry("360x160")
    popup.resizable(False, False)
    popup.configure(fg_color="#1a1a1a")
    popup.grab_set()

    app.update_idletasks()
    x = app.winfo_x() + (app.winfo_width() // 2) - 180
    y = app.winfo_y() + (app.winfo_height() // 2) - 80
    popup.geometry(f"360x160+{x}+{y}")

    ctk.CTkLabel(popup, text="⚠", font=(FONT, 28), text_color="#e05c5c").pack(pady=(18, 0))
    ctk.CTkLabel(popup, text=message, font=(FONT, 14),
                 text_color="#b3b3b3", wraplength=300).pack(pady=(6, 0))
    ctk.CTkButton(popup, text="OK", font=(FONT, 14),
                  text_color="#1a1a1a", fg_color="#b3b3b3", hover_color="#ffffff",
                  width=80, height=30, command=popup.destroy).pack(pady=(12, 0))

def run_binary(file_path, method_key, action):
    """
    action: "compress" | "decompress"
    Returns bytes output from the binary.
    """
    method = METHOD_MAP.get(method_key)
    if method is None:
        raise ValueError(f"'{method_key}' is not supported.")

    if not os.path.isfile(BINARY_PATH):
        raise FileNotFoundError(f"Binary not found at: {BINARY_PATH}")

    result = subprocess.run(
        [BINARY_PATH, method, action, file_path],
        capture_output=True,
        text=False
    )
    if result.returncode != 0:
        err = result.stderr.decode("utf-8", errors="replace").strip() or "Unknown error."
        raise RuntimeError(f"Binary exited with code {result.returncode}:\n{err}")
    return result.stdout  # bytes

def app_setup():
    app = ctk.CTk()
    app.geometry(APP_SIZE)
    app.title(APP_TITLE)
    app.resizable(False, False)
    app.configure(fg_color=LEFT_COLOR)
    app.columnconfigure(0, weight=1)
    app.columnconfigure(1, weight=1)
    app.rowconfigure(0, weight=1)
    app.iconbitmap("icon.ico")
    return app

def create_widget(mstr, widget_type, **options):
    return widget_type(master=mstr, **options)

# ─── Ortak panel fabrikası ────────────────────────────────────────────────────
def build_panel(app, column, bg_color, title, action,
                save_extension, save_filetypes):
    """
    action : "compress" | "decompress"
    """
    last_output_bytes = [None]   # list so inner functions can mutate it

    def set_ui_locked(locked):
        state = "disabled" if locked else "normal"
        upload_btn.configure(state=state)
        action_btn.configure(
            state=state,
            text=f"{title}ing..." if locked else title,
            fg_color="#555555" if locked else PRIMARY_COLOR,
        )

    def upload_file():
        path = filedialog.askopenfilename(
            title="Select a file",
            filetypes=ALL_FILETYPES,
        )
        if not path:
            return
        upload_btn._file_path = path
        file_label.configure(text=path)

    def on_action():
        file_path = getattr(upload_btn, "_file_path", None)
        if not file_path or not os.path.isfile(file_path):
            show_error(app, "No file selected.\nPlease browse and select a file first.")
            return

        method_key = method_dropdown.get()

        set_ui_locked(True)
        app.update()
        try:
            output = run_binary(file_path, method_key, action)
            last_output_bytes[0] = output
            size_label.configure(text=format_size(len(output)))
        except FileNotFoundError as e:
            show_error(app, str(e))
            size_label.configure(text="—")
            last_output_bytes[0] = None
        except RuntimeError as e:
            show_error(app, str(e))
            size_label.configure(text="—")
            last_output_bytes[0] = None
        except Exception as e:
            show_error(app, f"Unexpected error:\n{e}")
            size_label.configure(text="—")
            last_output_bytes[0] = None
        finally:
            set_ui_locked(False)

    def on_save():
        if not last_output_bytes[0]:
            show_error(app, f"No output to save.\nPlease {action} a file first.")
            return
        path = filedialog.asksaveasfilename(
            defaultextension=save_extension,
            filetypes=save_filetypes,
            title="Save Output File",
        )
        if not path:
            return
        # Kullanıcı uzantı yazmadıysa ve action decompress ise uzantıyı zorlamıyoruz
        try:
            with open(path, "wb") as f:
                f.write(last_output_bytes[0])
        except Exception as e:
            show_error(app, f"Failed to save file:\n{e}")

    # ── Layout ──
    panel = create_widget(
        app, ctk.CTkFrame,
        corner_radius=0, fg_color=bg_color, border_width=0,
    )
    panel.grid(row=0, column=column, sticky="nsew")
    panel.columnconfigure(0, weight=1)
    for r in range(4):
        panel.rowconfigure(r, weight=1)

    # Başlık
    create_widget(panel, ctk.CTkLabel,
                  text=title, font=(FONT, 50),
                  text_color=PRIMARY_COLOR).grid(row=0, column=0)

    # Dosya seçim kutusu
    file_frame = create_widget(
        panel, ctk.CTkFrame,
        corner_radius=10, fg_color=FRAME_COLOR,
        border_width=1, border_color=PRIMARY_COLOR,
    )
    file_frame.grid(row=1, column=0, pady=(0, 10))
    file_frame.columnconfigure(0, weight=1)

    upload_btn = create_widget(
        file_frame, ctk.CTkButton,
        text="Browse File", font=(FONT, 18),
        text_color=BUTTON_TEXT_COLOR, fg_color=PRIMARY_COLOR,
        hover_color=BUTTON_HOVER_COLOR, border_width=0,
        command=upload_file,
    )
    upload_btn.grid(row=0, column=0, padx=20, pady=(12, 6))

    file_label = create_widget(
        file_frame, ctk.CTkLabel,
        text="No file selected",
        font=(FONT, 12), text_color="#888888",
        wraplength=440,
    )
    file_label.grid(row=1, column=0, padx=10, pady=(0, 12))

    # Yöntem + Aksiyon butonu
    mid_frame = create_widget(
        panel, ctk.CTkFrame,
        fg_color="transparent", border_width=0,
    )
    mid_frame.grid(row=2, column=0, pady=10)

    method_dropdown = create_widget(
        mid_frame, ctk.CTkOptionMenu,
        values=list(METHOD_MAP.keys()),
        font=(FONT, 14), fg_color=FRAME_COLOR,
        button_color=PRIMARY_COLOR, button_hover_color=BUTTON_HOVER_COLOR,
        text_color=PRIMARY_COLOR, width=170,
    )
    method_dropdown.grid(row=0, column=0, padx=(0, 10))

    action_btn = create_widget(
        mid_frame, ctk.CTkButton,
        text=title, text_color=BUTTON_TEXT_COLOR,
        font=(FONT, 18), fg_color=PRIMARY_COLOR,
        hover_color=BUTTON_HOVER_COLOR, border_width=0,
        height=40, width=200, command=on_action,
    )
    action_btn.grid(row=0, column=1)

    # Sonuç: boyut + kaydet
    result_frame = create_widget(
        panel, ctk.CTkFrame,
        corner_radius=10, fg_color=FRAME_COLOR,
        border_width=1, border_color=PRIMARY_COLOR,
    )
    result_frame.grid(row=3, column=0, pady=10)
    result_frame.columnconfigure(0, weight=1)

    create_widget(result_frame, ctk.CTkLabel,
                  text="Output Size:",
                  font=(FONT, 16), text_color="#888888"
                  ).grid(row=0, column=0, pady=(12, 0))

    size_label = create_widget(
        result_frame, ctk.CTkLabel,
        text="—", font=(FONT, 32, "bold"), text_color=PRIMARY_COLOR,
    )
    size_label.grid(row=1, column=0, pady=(4, 8))

    btn_label = "Save File" if not save_extension else f"Save as {save_extension}"
    create_widget(
        result_frame, ctk.CTkButton,
        text=btn_label, text_color=BUTTON_TEXT_COLOR,
        fg_color=PRIMARY_COLOR, hover_color=BUTTON_HOVER_COLOR,
        border_width=0, font=(FONT, 16), command=on_save,
    ).grid(row=2, column=0, pady=(0, 12))

# ─── Başlat ───────────────────────────────────────────────────────────────────
def initialize():
    app = app_setup()

    # Dikey ayırıcı çizgi
    separator = create_widget(
        app, ctk.CTkFrame,
        width=1, fg_color="#2a2a2a", corner_radius=0,
    )
    separator.grid(row=0, column=0, sticky="nse", padx=(0, 0))
    separator.lift()

    build_panel(
        app,
        column=0,
        bg_color=LEFT_COLOR,
        title="Compress",
        action="compress",
        save_extension=".rle",
        save_filetypes=[
            ("RLE Compressed File", "*.rle"),
            ("All Files", "*.*"),
        ],
    )

    build_panel(
        app,
        column=1,
        bg_color=RIGHT_COLOR,
        title="Decompress",
        action="decompress",
        save_extension="",
        save_filetypes=[("All Files", "*.*")],
    )

    app.mainloop()

initialize()
import customtkinter as ctk
import satellite_panel
import json
import requests

ctk.set_appearance_mode("dark")
ctk.set_default_color_theme("blue")

APP_SIZE = "600x400"
APP_TITLE = "Authentication Panel"
FONT = "Trebuchet MS"

BG_COLOR = "#0f0f0f"
FRAME_COLOR = "#1a1a1a"
PRIMARY_COLOR = "#b3b3b3"
SECONDARY_TEXT = "#888888"

BUTTON_TEXT_COLOR = "#1a1a1a"
BUTTON_HOVER_COLOR = "#FFFFFF"

ACCOUNTS = []
account_entry = None
password_entry = None
server_entry = None
status_label = None
app = None
jwt_token = None

def create_widget(master, widget, **kwargs):
    return widget(master=master, **kwargs)

def app_setup():
    global app
    app = ctk.CTk()
    app.geometry(APP_SIZE)
    app.title(APP_TITLE)
    app.resizable(False, False)
    app.configure(fg_color=BG_COLOR)
    app.iconbitmap("icon.ico")
    app.columnconfigure(0, weight=1)
    app.rowconfigure(0, weight=1)
    return app
'''
def handle_login():
    username = account_entry.get()
    password = password_entry.get()
    url = server_entry.get()

    print(username," : ",password)

    session = requests.Session()

    payload = {
        "username": username,
        "password": password,
    }

    headers = {
        "Content-Type": "application/json",
        "Accept": "application/json",
        "User-Agent": "Mozilla/5.0"
    }

    try:
        response = session.post(f"https://{url}/login", json=payload, headers=headers, timeout=10)

        try:
            data = response.json()
        except requests.exceptions.JSONDecodeError:
            data = response.text 
            print(data)
        status_label.configure(text="Succesfully Logged In", text_color="green")
        jwt_token = response.json()["accessToken"]
        print(jwt_token)

    except requests.exceptions.ConnectionError:
        status_label.configure(text="Connection Error", text_color="red")
    
        print(e)
    except requests.exceptions.Timeout:
        status_label.configure(text="Timeout", text_color="red")
        print(e)
    except requests.exceptions.RequestException as e:
        status_label.configure(text=f"Access Denied\n{e}", text_color="red")
        print(e)
'''
'''
def build_ui(app):
    global account_entry, password_entry, server_entry, status_label

    main_frame = create_widget(app, ctk.CTkFrame, corner_radius=0, fg_color=BG_COLOR)
    main_frame.grid(row=0, column=0, sticky="nsew")
    main_frame.columnconfigure(0, weight=1)
    for i in range(8):
        main_frame.rowconfigure(i, weight=1)

    title = create_widget(main_frame, ctk.CTkLabel, text="Login Panel", font=(FONT, 40), text_color=PRIMARY_COLOR)
    title.grid(row=0, column=0)

    login_frame = create_widget(main_frame, ctk.CTkFrame, corner_radius=10, fg_color=FRAME_COLOR,
                                border_width=1, border_color=PRIMARY_COLOR)
    login_frame.grid(row=1, column=0, padx=100, pady=10)
    login_frame.columnconfigure(0, weight=1)

    acc_label = create_widget(login_frame, ctk.CTkLabel, text="Account Name", font=(FONT, 16), text_color=SECONDARY_TEXT)
    acc_label.grid(row=0, column=0, pady=(10, 0))
    account_entry = create_widget(login_frame, ctk.CTkEntry, placeholder_text="Enter username",
                                  border_color=PRIMARY_COLOR, fg_color=BG_COLOR, width=200)
    account_entry.grid(row=1, column=0, padx=20, pady=(5, 10))

    pass_label = create_widget(login_frame, ctk.CTkLabel, text="Password", font=(FONT, 16), text_color=SECONDARY_TEXT)
    pass_label.grid(row=2, column=0)
    password_entry = create_widget(login_frame, ctk.CTkEntry, placeholder_text="Enter password", show="*",
                                   border_color=PRIMARY_COLOR, fg_color=BG_COLOR, width=200)
    password_entry.grid(row=3, column=0, padx=20, pady=(5, 10))

    server_label = create_widget(login_frame, ctk.CTkLabel, text="Server ID", font=(FONT, 16), text_color=SECONDARY_TEXT)
    server_label.grid(row=4, column=0)
    server_entry = create_widget(login_frame, ctk.CTkEntry, placeholder_text="Enter server ID",
                                 border_color=PRIMARY_COLOR, fg_color=BG_COLOR, width=200)
    server_entry.grid(row=5, column=0, padx=20, pady=(5, 10))

    login_button = create_widget(login_frame, ctk.CTkButton, text="Login", fg_color=PRIMARY_COLOR,
                                 text_color=BUTTON_TEXT_COLOR, hover_color=BUTTON_HOVER_COLOR,
                                 border_width=0, font=(FONT, 16), command=handle_login)
    login_button.grid(row=6, column=0, padx=20, pady=(5, 10))

    status_label = create_widget(login_frame, ctk.CTkLabel, text="", font=(FONT, 13), text_color=SECONDARY_TEXT)
    status_label.grid(row=7, column=0, pady=(0, 10))

    footer = create_widget(main_frame, ctk.CTkLabel, text="Provided by 🦅", font=(FONT, 13), text_color=SECONDARY_TEXT)
    footer.grid(row=5, column=0, pady=(0, 10))'''

def initialize():
    global ACCOUNTS
    # Load accounts from JSON
    with open("ACCOUNTS.json", "r", encoding="utf-8") as f:
        ACCOUNTS = json.load(f)  # List of dicts

    app = app_setup()
    #build_ui(app)
    app.mainloop()

initialize()
import streamlit as st
import subprocess
import os

def run_compiler(code: str):
    with open("input.c", "w") as f:
        f.write(code)

    result = subprocess.run(["./algoGenie"], capture_output=True, text=True)

    if result.returncode != 0:
        return "", "", "", f" Error:\n{result.stderr}"

    parts = result.stdout.split("===END===\n")
    
    pseudocode = parts[0].replace("Pseudocode:\n", "").strip() if len(parts) > 0 else ""
    tokens = parts[1].replace("Tokens:\n", "").strip() if len(parts) > 1 else ""
    tree_path = "tree.html"
    error = ""

    return pseudocode, tokens, tree_path, error



st.set_page_config(layout="wide")
st.title("C to Pseudocode + Parse Tree Visualizer")


code_input = st.text_area("Enter your C code below:", height=250)

if st.button(" Compile and Visualize"):
    if code_input.strip():
        pseudocode, tokens, tree_path, error = run_compiler(code_input)

        if error:
            st.error(error)
        else:
            col1, col2, col3 = st.columns([1, 1, 1])

            with col1:
                st.subheader(" Pseudocode")
                st.code(pseudocode, language="text")

            with col2:
                st.subheader(" Parse Tree")
                try:
                    with open(tree_path, "r") as f:
                        html = f.read()
                        st.components.v1.html(html, height=500, scrolling=True)
                except:
                    st.warning("Parse tree not found.")

            with col3:
                st.subheader(" Tokens")
                st.code(tokens, language="text")
    else:
        st.warning("Please enter some C code to compile.")

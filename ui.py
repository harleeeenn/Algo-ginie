import streamlit as st
import subprocess

def run_compiler(code: str) -> str:
    with open("input.c", "w") as f:
        f.write(code)

    result = subprocess.run(["./algoGenie", "input.c"],
                            capture_output=True, text=True)

    return result.stdout if result.returncode == 0 else result.stderr

st.title(" C to Pseudocode Converter")

st.markdown("Paste your C code below:")
code_input = st.text_area("C Code", height=300)

uploaded_file = st.file_uploader("Or upload a `.c` file", type="c")

if uploaded_file is not None:
    code_input = uploaded_file.read().decode("utf-8")
    st.text_area("Uploaded Code", code_input, height=300)

if st.button("Convert to Pseudocode"):
    if code_input.strip():
        output = run_compiler(code_input)
        st.markdown("Pseudocode Output")
        st.code(output, language="text")
    else:
        st.error("Please provide some C code.")


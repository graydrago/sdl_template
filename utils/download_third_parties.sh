if [ -d ../third_parties ]; then
  echo "Directory ../third_parties exists."
else
  mkdir ../third_parties
  echo "Directory ../third_parties has been created."
fi

if [ -d ../third_parties/glm ]; then
  echo "glm library is found."
else
  echo "Cloning glm library has been started."
  git clone --branch 0.9.8 --depth 1 --single-branch https://github.com/g-truc/glm.git ../third_parties/glm/
  echo "Cloning glm library has been done."
fi


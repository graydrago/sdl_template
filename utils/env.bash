DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/"
if [[ ":$PATH:" != *":${DIR}:"* ]]; then
  PATH="${DIR}:${PATH}"
  echo "${DIR} added to PATH"
else
  echo "${DIR} is already in PATH"
fi


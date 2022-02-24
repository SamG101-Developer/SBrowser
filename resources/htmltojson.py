import xmltodict
from lxml import etree,html
from pprint import pformat

htmlcode = """
...
"""

xhtml = etree.tostring(html.fromstring(htmlcode))
json = xmltodict.parse(xhtml)
print(xhtml)
